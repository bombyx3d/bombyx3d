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
#include "YamlParser.h"
#include "io/streams/FileInputStream.h"
#include "io/FileSystem.h"
#include "utility/debug.h"
#include <cstring>

namespace Z
{
    YamlParser::YamlParser()
    {
        memset(&m_Event, 0, sizeof(m_Event));
    }

    YamlParser::~YamlParser()
    {
        cleanup();
    }

    void YamlParser::cleanup()
    {
        cleanupEvent();

        if (m_ParserValid) {
            yaml_parser_delete(&m_Parser);
            m_ParserValid = false;
        }
    }

    void YamlParser::cleanupEvent()
    {
        if (m_EventValid) {
            yaml_event_delete(&m_Event);
            memset(&m_Event, 0, sizeof(m_Event));
            m_EventValid = false;
        }
    }

    const yaml_mark_t& YamlParser::problemMark()
    {
        if (!m_ParserValid)
            memset(&m_Parser.problem_mark, 0, sizeof(m_Parser.problem_mark));
        return m_Parser.problem_mark;
    }

    const yaml_event_t& YamlParser::current()
    {
        return m_Event;
    }

    bool YamlParser::open(const std::string& path)
    {
        FileReaderPtr reader = FileSystem::defaultFileSystem()->openFile(path);
        if (!reader)
            return false;
        return open(reader);
    }

    bool YamlParser::open(const FileReaderPtr& reader)
    {
        FileInputStream stream(reader);
        return open(&stream);
    }

    bool YamlParser::open(const InputStreamPtr& stream)
    {
        return open(stream.get());
    }

    bool YamlParser::open(InputStream* stream)
    {
        cleanup();

        Z_CHECK(stream != nullptr);
        if (!stream)
            return false;

        if (!yaml_parser_initialize(&m_Parser))
        {
            Z_LOG("Unable to initialize YAML parser.");
            return false;
        }

        m_ParserValid = true;
        m_FileName = stream->name();

        auto reader = [](void* ud, unsigned char* buffer, size_t size, size_t * bytesRead) -> int {
            InputStream* input = reinterpret_cast<InputStream*>(ud);
            return input->readAtMost(buffer, size, bytesRead);
        };
        yaml_parser_set_input(&m_Parser, reader, stream);

        return true;
    }

    bool YamlParser::next()
    {
        if (!m_ParserValid)
            return false;

        cleanupEvent();

        if (yaml_parser_parse(&m_Parser, &m_Event))
            m_EventValid = true;
        else
            m_Event.start_mark = m_Parser.problem_mark;

        return m_EventValid;
    }
}
