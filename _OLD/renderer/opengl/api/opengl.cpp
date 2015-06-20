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
#include "opengl.h"
#include "core/utility/debug.h"
#include <algorithm>
#include <string>
#include <cstring>

#ifdef Z_TARGET_QT5
 #include "qt5/opengl.h"
#elif defined(Z_OPENGL_FOUND)
 #include "normal/opengl.h"
#else
 #include "dummy/opengl.h"
#endif

#define Z_GL_EXTENSION_(X)
#include "extensions.h"
#undef Z_GL_EXTENSION_

static const char* g_OpenGLVersion;
static bool g_IsOpenGLES;
static int g_OpenGLMajorVersion = -1;
static int g_OpenGLMinorVersion;
static int g_OpenGLReleaseVersion;

const char* gl::GetVersionString()
{
    if (!g_OpenGLVersion) {
        g_OpenGLVersion = gl::GetString(GL::VERSION);
        if (!g_OpenGLVersion)
            Z_LOG("Unable to retrieve OpenGL version string.");
        else
            Z_LOG("OpenGL version: " << g_OpenGLVersion);
    }
    return g_OpenGLVersion;
}

void gl::GetVersionNumber(int* major, int* minor, int* release, bool* es)
{
    if (g_OpenGLMajorVersion < 0) {
        g_OpenGLMajorVersion = -1;
        g_OpenGLMinorVersion = 0;
        g_OpenGLReleaseVersion = 0;
        g_IsOpenGLES = false;

        const char* version = gl::GetVersionString();
        if (version) {
            static const char esPrefix[] = "OpenGL ES";
            const size_t esPrefixLength = strlen(esPrefix);

            size_t length = strlen(version);
            if (length > esPrefixLength && !memcmp(version, esPrefix, length)) {
                g_IsOpenGLES = true;

                version += esPrefixLength;
                if (*version == '-') {
                    ++version;
                    while (*version && *version != ' ')
                        ++version;
                }

                while (*version == ' ')
                    ++version;
            }

            if (sscanf(version, "%d.%d.%d",
                &g_OpenGLMajorVersion, &g_OpenGLMinorVersion, &g_OpenGLReleaseVersion) != 3)
            {
                g_OpenGLReleaseVersion = 0;
                if (sscanf(version, "%d.%d", &g_OpenGLMajorVersion, &g_OpenGLMinorVersion) != 2) {
                    g_OpenGLMinorVersion = 0;
                    if (sscanf(version, "%d", &g_OpenGLMajorVersion) != 1)
                        g_OpenGLMajorVersion = 0;
                }
            }

            Z_LOG("Decoded OpenGL version: " << g_OpenGLMajorVersion << '.' << g_OpenGLMinorVersion << '.'
                << g_OpenGLReleaseVersion << ' ' << (g_IsOpenGLES ? "ES" : "Desktop"));
        }
    }

    if (g_OpenGLMajorVersion < 0)
        Z_LOG("Unable to decode OpenGL version string.");

    if (major)
        *major = std::max(0, g_OpenGLMajorVersion);
    if (minor)
        *minor = g_OpenGLMinorVersion;
    if (release)
        *release = g_OpenGLReleaseVersion;
    if (es)
        *es = g_IsOpenGLES;
}

bool gl::isOpenGLES(int major, int minor)
{
    int glMajor = 0, glMinor = 0;
    bool isES = false;
    GetVersionNumber(&glMajor, &glMinor, nullptr, &isES);
    return isES && (glMajor > major || (glMajor == major && glMinor >= minor));
}

bool gl::isDesktopOpenGL(int major, int minor, int release)
{
    int glMajor = 0, glMinor = 0, glRelease = 0;
    bool isES = true;
    GetVersionNumber(&glMajor, &glMinor, &glRelease, &isES);
    return !isES && (glMajor > major || (glMajor == major &&
        (glMinor > minor || (glMinor == minor && glRelease >= release))));
}

bool gl::Supports2DArrayTextures()
{
    return isOpenGLES(3) || isDesktopOpenGL(3) || gl::IsExtensionSupported(GL_EXT_texture_array);
}

bool gl::Supports3DTextures()
{
    return isOpenGLES(3) || isDesktopOpenGL(1, 2);
}

GL::Int gl::GetInteger(GL::Enum param)
{
    GL::Int value = 0;
    gl::GetIntegerv(param, &value);
    return value;
}

void gl::EnableOrDisable(GL::Enum cap, bool flag)
{
    if (flag)
        gl::Enable(cap);
    else
        gl::Disable(cap);
}

bool gl::IsExtensionSupported(const char* name)
{
    const char* extensions = gl::GetString(GL::EXTENSIONS);
    if (!extensions) {
        Z_LOG("Checking for OpenGL extension " << name << ": NOT FOUND.");
        return false;
    }

    size_t nameLength = strlen(name);
    const char* start = extensions;
    for (;;) {
        const char* p = strstr(start, name);
        if (!p)
            break;

        if ((p == extensions || p[-1] == ' ') && (p[nameLength] == ' ' || p[nameLength] == 0)) {
            Z_LOG("Checking for OpenGL extension " << name << ": found.");
            return true;
        }

        start = p + nameLength;
        while (*start && *start != ' ')
            ++start;
    }

    Z_LOG("Checking for OpenGL extension " << name << ": NOT FOUND.");
    return false;
}

bool gl::IsExtensionSupported(GLExtension extension)
{
    #define Z_GL_EXTENSION_(X) static char X##_supported = -1;
    #include "extensions.h"
    #undef Z_GL_EXTENSION_

    switch (extension)
    {
    #define Z_GL_EXTENSION_(X) \
        case X: \
            if (X##_supported == -1) \
                X##_supported = char(gl::IsExtensionSupported(#X) ? 1 : 0); \
            return X##_supported != 0;
    #include "extensions.h"
    #undef Z_GL_EXTENSION_
    }

    Z_ASSERT(false);
    return false;
}

void gl3::BindTexture(GL3::Enum target, GL::UInt texture)
{
    gl::BindTexture(GL::Enum(target), texture);
}

void gl3::GetIntegerv(GL3::Enum pname, GL::Int* params)
{
    gl::GetIntegerv(GL::Enum(pname), params);
}
