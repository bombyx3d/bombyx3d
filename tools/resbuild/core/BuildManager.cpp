/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "BuildManager.h"
#include "project/BuildRule.h"
#include "project/BuildProject.h"
#include "zip/ZipReader.h"
#include "zip/ZipWriter.h"
#include <zip.h>
#include <sys/stat.h>
#include <sstream>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <algorithm>

extern "C" {
uLong filetime(const char *filename, tm_zip *tmzip, uLong *dostime);
}

BuildManager::BuildManager()
    : m_BuildActive(false)
    , m_ShouldAbort(false)
{
}

BuildManager::~BuildManager()
{
    requestAbort();
    wait();
}

bool BuildManager::startBuild(const std::string& outputFile, const BuildProject* project, Mode mode,
    Notifications* notifications)
{
    requestAbort();
    wait();

    m_ShouldAbort.store(false);
    m_BuildActive.store(true);

    m_Notifications = notifications;
    m_Project = project;
    m_OutputFile = outputFile;
    m_Mode = mode;
    m_BuildThread.reset(new std::thread(std::bind(&BuildManager::managementThread, this)));

    return true;
}

void BuildManager::requestAbort()
{
    m_ShouldAbort.store(true);
}

void BuildManager::wait()
{
    if (m_BuildThread) {
        m_BuildThread->join();
        m_BuildThread.reset();
    }
}

void BuildManager::checkAbort()
{
    if (shouldAbort())
        throw Abort();
}

void BuildManager::managementThread()
{
    try {
        checkAbort();

        m_Notifications->setStatus(AnalyzingDependencies);
        m_Notifications->setProgress(0, 0);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // See if target ZIP file already exists. If it does, open it for reading.

        ZipReader zipReader(m_Notifications);
        if (m_Mode == FULL_OPTIMIZED_BUILD)
            m_Notifications->printVerbose("Performing a full optimized build.");
        else if (m_Mode == FULL_DRAFT_BUILD)
            m_Notifications->printVerbose("Performing a full draft build.");
        else if (m_Mode == INCREMENTAL_DRAFT_BUILD) {
            struct stat st;
            int r = stat(m_OutputFile.c_str(), &st);
            if (r < 0) {
                std::stringstream ss;
                ss << "File \"" << m_OutputFile << "\" does not exist. Performing a full draft build.";
                m_Notifications->printVerbose(ss.str());
            } else {
                if (!zipReader.open(m_OutputFile)) {
                    std::stringstream ss;
                    ss << "Unable to open file \"" << m_OutputFile << "\".";
                    throw std::runtime_error(ss.str());
                }

                std::stringstream ss;
                ss << "Performing an incremental draft build.";
                m_Notifications->printVerbose(ss.str());
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Create a temporary output file

        checkAbort();
        m_TemporaryFile = m_OutputFile + '~';

        // If temporary file already exists, delete it
        struct stat st;
        int r = stat(m_TemporaryFile.c_str(), &st);
        if (r == 0) {
            r = remove(m_TemporaryFile.c_str());
            if (r < 0) {
                int err = errno;
                std::stringstream ss;
                ss << "Unable to delete file \"" << m_TemporaryFile << "\" (errno " << err << ").";
                throw std::runtime_error(ss.str());
            }
        }

        // Create temporary file
        ZipWriter zipWriter(m_Notifications);
        if (!zipWriter.open(m_TemporaryFile)) {
            std::stringstream ss;
            ss << "Unable to create file \"" << m_TemporaryFile << "\".";
            throw std::runtime_error(ss.str());
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Analyze dependencies if we are performing an incremental build

        checkAbort();

        std::vector<std::string> unmodifiedFiles;
        std::vector<BuildRulePtr> rules;
        rules.reserve(m_Project->rules().size());

        if (m_Mode != INCREMENTAL_DRAFT_BUILD || !zipReader.isOpen())
            rules = m_Project->rules();
        else {
            // Iterate over all rules and collect: (a) rules to be rebuild, and (b) unmodified output files
            m_Notifications->setProgress(0, double(m_Project->rules().size()));
            double index = 0;
            for (const auto& rule : m_Project->rules()) {
                if (rule->inputFiles().empty() || rule->outputFiles().empty()) {
                    // Always build rules that have no input files and/or no output files
                    rules.push_back(rule);
                    continue;
                }

                checkAbort();

                std::stringstream ss;
                ss << "Analyzing dependencies for rule \"" << rule->name() << "\".";
                m_Notifications->printVerbose(ss.str());

                index += 1;
                m_Notifications->setProgress(index, double(m_Project->rules().size()));

                // Determine latest modification time for input files

                bool shouldRebuild = false;
                uLong modificationTime = 0;
                for (const auto& inputFile : rule->inputFiles()) {
                    tm_zip zipTime;
                    uLong dosTime = 0;
                    auto r = filetime(inputFile.c_str(), &zipTime, &dosTime);
                    if (!r) {
                        ss.str(std::string());
                        ss << "Unable to retrieve last modification time for file \"" << inputFile << "\".";
                        m_Notifications->printWarning(ss.str());
                        shouldRebuild = true;
                        break;
                    }
                    modificationTime = std::max(modificationTime, dosTime);
                    checkAbort();
                }

                // See if input files were modified

                if (!shouldRebuild) {
                    ZipExtraField extraField;
                    std::string settingsHash = rule->settingsHash();
                    for (const auto& outputFile : rule->outputFiles()) {
                        checkAbort();

                        if (!zipReader.seekToFile(outputFile.c_str())) {
                            shouldRebuild = true;
                            break;
                        }

                        if (!zipReader.readExtraField(&extraField)) {
                            shouldRebuild = true;
                            break;
                        }

                        if (extraField.buildTime < modificationTime) {
                            shouldRebuild = true;
                            break;
                        }

                        if (extraField.settingsHash != settingsHash) {
                            shouldRebuild = true;
                            break;
                        }
                    }
                }

                // See if output file should be rebuilt

                if (!shouldRebuild)
                    unmodifiedFiles.insert(unmodifiedFiles.end(), rule->outputFiles().begin(), rule->outputFiles().end());
                else
                    rules.emplace_back(rule);
            }
        }

        // Copy all unmodified files into the output archive

        if (!unmodifiedFiles.empty()) {
            m_Notifications->setStatus(CopyingUnmodifiedFiles);
            m_Notifications->setProgress(0, double(unmodifiedFiles.size()));
            double index = 0;
            for (const auto& fileName : unmodifiedFiles) {
                checkAbort();

                std::stringstream ss;
                ss << "Preserving unmodified file \"" << fileName << "\".";
                m_Notifications->printVerbose(ss.str());

                index += 1;
                m_Notifications->setProgress(index, double(unmodifiedFiles.size()));

                if (!zipWriter.addCurrentFileFromZip(&zipReader)) {
                    ss.str(std::string());
                    ss << "Unable to copy entry \"" << fileName << "\" from ZIP file \"" << zipReader.fileName()
                        << "\" to ZIP file \"" << zipWriter.fileName() << "\".";
                    throw std::runtime_error(ss.str());
                }
            }
        }

        // Remove original file and rename temporary file

        m_Notifications->setStatus(Finishing);
        m_Notifications->setProgress(0, 0);

        if (zipReader.isOpen())
            zipReader.close();

        remove(m_OutputFile.c_str());

        r = rename(m_TemporaryFile.c_str(), m_OutputFile.c_str());
        if (r < 0) {
            int err = errno;
            std::stringstream ss;
            ss << "Unable to rename file \"" << m_TemporaryFile << "\" to \"" << m_OutputFile
                << "\" (errno " << err << ").";
            throw std::runtime_error(ss.str());
        }
    } catch (const Abort&) {
        m_Notifications->buildAborted();
        return;
    } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "Unhandled exception: " << e.what();
        m_Notifications->printError(ss.str());
        m_Notifications->buildFailed();
        return;
    } catch (...) {
        std::stringstream ss;
        ss << "Unhandled exception.";
        m_Notifications->printError(ss.str());
        m_Notifications->buildFailed();
        return;
    }

    m_Notifications->buildSucceeded();
}
