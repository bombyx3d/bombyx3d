
set(source_files)
set(defines)
set(libraries)

if(WIN32)
    list(APPEND defines "Z_PLATFORM_WIN32")
    list(APPEND libraries glfw)
    list(APPEND source_files
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        win32/main.cpp
        win32/WinAPI.h
    )
endif()

add_library(platform STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(platform PROPERTIES CXX_STANDARD 11)
target_link_libraries(platform engine glm ${libraries})
target_compile_definitions(platform PUBLIC ${defines})
