
#pragma once

#ifdef __APPLE__
 #include <OpenGL/gl.h>
#elif defined(_WIN32)
 #include "engine/platform/win32/WinAPI.h"
 #include <GL/gl.h>
#else
 #include <GL/gl.h>
#endif
