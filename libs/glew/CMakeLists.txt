
project(glew C)
include(../../cmake/DisableWarnings.cmake)

find_package(OpenGL)

if(OPENGL_INCLUDE_DIR)
    include_directories(${OPENGL_INCLUDE_DIR})
endif()

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" src/*.c include/GL/*.h)
b3d_set_source_groups(${source_files})

b3d_add_library(glew STATIC EXCLUDE_FROM_ALL ${source_files})
target_include_directories("${glew}" BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions("${glew}" PUBLIC "GLEW_STATIC" "GLEW_NO_GLU")

if(OPENGL_gl_LIBRARY)
    target_link_libraries("${glew}" ${OPENGL_gl_LIBRARY})
endif()
