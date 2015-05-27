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

#pragma once
#include <atomic>
#include <thread>
#include <memory>
#include <string>

class BuildProject;

class BuildManager
{
public:
    enum Status
    {
        AnalyzingDependencies,
        CopyingUnmodifiedFiles,
        Finishing,
    };

    enum Mode
    {
        FULL_OPTIMIZED_BUILD,
        FULL_DRAFT_BUILD,
        INCREMENTAL_DRAFT_BUILD,
    };

    class Notifications
    {
    public:
        virtual ~Notifications() = default;

        virtual void buildSucceeded() {}
        virtual void buildFailed() {}
        virtual void buildAborted() {}

        virtual void setStatus(Status) {}
        virtual void setProgress(double current, double maximum) { (void)current; (void)maximum; }

        virtual void printVerbose(const std::string&) {}
        virtual void printInfo(const std::string&) {}
        virtual void printWarning(const std::string&) {}
        virtual void printError(const std::string&) {}
    };

    class Abort
    {
    };


    BuildManager();
    ~BuildManager();

    bool isBuildActive() const { return m_BuildActive.load(); }
    bool shouldAbort() const { return m_ShouldAbort.load(); }

    bool startBuild(const std::string& targetFile, const BuildProject* project, Mode mode,
        Notifications* notifications);

    void requestAbort();
    void wait();

    // methods for builders
    void checkAbort();

private:
    std::atomic<bool> m_BuildActive;
    std::atomic<bool> m_ShouldAbort;
    std::unique_ptr<std::thread> m_BuildThread;
    std::string m_OutputFile;
    std::string m_TemporaryFile;
    const BuildProject* m_Project;
    Notifications* m_Notifications;
    Mode m_Mode;

    void managementThread();

    BuildManager(const BuildManager&) = delete;
    BuildManager& operator=(const BuildManager&) = delete;
};
