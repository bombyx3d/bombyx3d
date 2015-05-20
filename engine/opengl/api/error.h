/*
 * Copyright (c) 2014 Nikolay Zapolnov (zapolnov@gmail.com).
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
#include "opengl.h"
#include "utility/debug.h"
#include <sstream>
#include <iostream>
#include <string>

#if !Z_ASSERTIONS_ENABLED || !Z_LOGGING_ENABLED
 #define Z_CHECK_GL_LOCATION(name, var, location)
 #define Z_CHECK_GL_ERROR0(name)
 #define Z_CHECK_GL_ERROR1(name, p)
 #define Z_CHECK_GL_ERROR2(name, p1, p2)
 #define Z_CHECK_GL_ERROR3(name, p1, p2, p3)
 #define Z_CHECK_GL_ERROR4(name, p1, p2, p3, p4)
 #define Z_CHECK_GL_ERROR5(name, p1, p2, p3, p4, p5)
 #define Z_CHECK_GL_ERROR6(name, p1, p2, p3, p4, p5, p6)
 #define Z_CHECK_GL_ERROR7(name, p1, p2, p3, p4, p5, p6, p7)
 #define Z_CHECK_GL_ERROR8(name, p1, p2, p3, p4, p5, p6, p7, p8)
 #define Z_CHECK_GL_ERROR9(name, p1, p2, p3, p4, p5, p6, p7, p8, p9)
 #define Z_CHECK_GLCLEAR_ERROR(name, p)
#else
 #define Z_CHECK_GL_LOCATION(name, var, location) { \
        if (location < 0) \
            Z_LOG(#name << "(\"" << var << "\") returned " << location << '.'); \
    }
 #define Z_CHECK_GL_ERROR_(name, params) { \
        GL::Enum err = gl::GetError(); \
        if (err != GL::NO_ERROR) \
            Z_LOG(#name << '(' << params << "): " << err << '.'); \
    }
 #define Z_CHECK_GL_ERROR0(name) Z_CHECK_GL_ERROR_(name, "")
 #define Z_CHECK_GL_ERROR1(name, p) Z_CHECK_GL_ERROR_(name, p)
 #define Z_CHECK_GL_ERROR2(name, p1, p2) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2)
 #define Z_CHECK_GL_ERROR3(name, p1, p2, p3) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3)
 #define Z_CHECK_GL_ERROR4(name, p1, p2, p3, p4) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4)
 #define Z_CHECK_GL_ERROR5(name, p1, p2, p3, p4, p5) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4 << ", " << p5)
 #define Z_CHECK_GL_ERROR6(name, p1, p2, p3, p4, p5, p6) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4 << ", " << p5 << ", " << p6)
 #define Z_CHECK_GL_ERROR7(name, p1, p2, p3, p4, p5, p6, p7) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4 << ", " << p5 << ", " << p6 << ", " << p7)
 #define Z_CHECK_GL_ERROR8(name, p1, p2, p3, p4, p5, p6, p7, p8) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4 << ", " << p5 << ", " << p6 << ", " << p7 << ", " << p8)
 #define Z_CHECK_GL_ERROR9(name, p1, p2, p3, p4, p5, p6, p7, p8, p9) Z_CHECK_GL_ERROR_(name, p1 << ", " << p2 << ", " << p3 << ", " << p4 << ", " << p5 << ", " << p6 << ", " << p7 << ", " << p8 << ", " << p9)
 #define Z_CHECK_GLCLEAR_ERROR(name, p) Z_CHECK_GL_ERROR_(name, gl::DecodeGLClearMask_(p))
 namespace std {
     ostream& operator<<(ostream& s, GL::Enum value);
     ostream& operator<<(ostream& s, GL::Boolean value);
 }
 namespace gl {
    std::string DecodeGLClearMask_(GL::Bitfield mask);
 }
#endif
