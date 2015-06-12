﻿/*
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
#include "io/files/FileReader.h"
#include "io/streams/InputStream.h"
#include <string>
#include <yaml.h>

namespace Z
{
    class YamlParser
    {
    public:
        YamlParser();
        ~YamlParser();

        const std::string& fileName() const { return m_FileName; }

        const yaml_mark_t& problemMark();
        const yaml_event_t& current();

        bool open(const std::string& path);
        bool open(const FileReaderPtr& reader);
        bool open(const InputStreamPtr& stream);
        bool open(InputStream* stream);

        bool next();

    private:
        yaml_parser_t m_Parser;
        yaml_event_t m_Event;
        bool m_ParserValid = false;
        bool m_EventValid = false;
        std::string m_FileName;

        void cleanup();
        void cleanupEvent();

        YamlParser(const YamlParser&) = delete;
        YamlParser& operator=(const YamlParser&) = delete;
    };
}