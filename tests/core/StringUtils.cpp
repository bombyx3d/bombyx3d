/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include "engine/core/StringUtils.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestAssert.h>

class StringUtilsTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(StringUtilsTest);
    CPPUNIT_TEST(testEmptyString);
    CPPUNIT_TEST(testSameString);
    CPPUNIT_TEST(testSuccess);
    CPPUNIT_TEST(testFailure);
    CPPUNIT_TEST_SUITE_END();

public:
    void testEmptyString()
    {
        bool result = StringUtils::endsWith("", "");
        CPPUNIT_ASSERT_EQUAL(true, result);

        result = StringUtils::endsWith(".", "");
        CPPUNIT_ASSERT_EQUAL(true, result);

        result = StringUtils::endsWith("LongString", "");
        CPPUNIT_ASSERT_EQUAL(true, result);
    }

    void testSameString()
    {
        bool result = StringUtils::endsWith(".", ".");
        CPPUNIT_ASSERT_EQUAL(true, result);

        result = StringUtils::endsWith("Hello", "Hello");
        CPPUNIT_ASSERT_EQUAL(true, result);
    }

    void testSuccess()
    {
        bool result = StringUtils::endsWith("LongString", "String");
        CPPUNIT_ASSERT_EQUAL(true, result);
    }

    void testFailure()
    {
        bool result = StringUtils::endsWith("A", "B");
        CPPUNIT_ASSERT_EQUAL(false, result);

        result = StringUtils::endsWith("", "A");
        CPPUNIT_ASSERT_EQUAL(false, result);

        result = StringUtils::endsWith("MiddleStringMiddle", "String");
        CPPUNIT_ASSERT_EQUAL(false, result);

        result = StringUtils::endsWith("StringLong", "String");
        CPPUNIT_ASSERT_EQUAL(false, result);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringUtilsTest);
