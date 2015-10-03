
#pragma once
#include "src/common/platform.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstdarg>
#include <cstddef>
#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <set>
#include <map>
#include <bitset>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <wrl.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#ifdef _MSC_VER
#include <wrl/async.h>
#include <Windows.System.Threading.h>
#include <windows.applicationmodel.core.h>
#include <windows.graphics.display.h>
#endif

#include "include/angle_gl.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLSLANG/ShaderLang.h>

#include "src/common/angleutils.h"
#include "src/common/BitSetIterator.h"
#include "src/common/debug.h"
#include "src/common/event_tracer.h"
#include "src/common/mathutil.h"
#include "src/common/matrix_utils.h"
#include "src/common/MemoryBuffer.h"
#include "src/common/Optional.h"
#include "src/common/string_utils.h"
#include "src/common/tls.h"
#include "src/common/utilities.h"
#include "src/common/version.h"

#include "src/libANGLE/angletypes.h"
#include "src/libANGLE/Buffer.h"
#include "src/libANGLE/Caps.h"
#include "src/libANGLE/Compiler.h"
#include "src/libANGLE/Config.h"
#include "src/libANGLE/Context.h"
#include "src/libANGLE/Error.h"
#include "src/libANGLE/Framebuffer.h"
#include "src/libANGLE/FramebufferAttachment.h"
#include "src/libANGLE/State.h"
#include "src/libANGLE/Shader.h"
#include "src/libANGLE/Query.h"
#include "src/libANGLE/Renderbuffer.h"
#include "src/libANGLE/Texture.h"
#include "src/libANGLE/TransformFeedback.h"
#include "src/libANGLE/Uniform.h"
#include "src/libANGLE/VertexArray.h"

#include "src/compiler/translator/Compiler.h"
