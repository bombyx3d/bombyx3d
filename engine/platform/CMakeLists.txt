
set(source_files)
set(defines)
set(libraries)

if(WIN32)
    list(APPEND defines "Z_PLATFORM_WIN32")
    list(APPEND libraries glfw)
    list(APPEND source_files
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        win32/main.cpp
        win32/WinAPI.h
    )
endif()

if(APPLE)
    list(APPEND defines "Z_PLATFORM_OSX")
    list(APPEND libraries glfw)
    list(APPEND source_files
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        osx/main.mm
    )
endif()

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    list(APPEND defines "Z_PLATFORM_LINUX")
    list(APPEND libraries glfw)
    list(APPEND source_files
        shared/GlfwWrapper.cpp
        shared/GlfwWrapper.h
        shared/StdIoFile.cpp
        shared/StdIoFile.h
        shared/StdIoFileSystem.cpp
        shared/StdIoFileSystem.h
        linux/main.cpp
    )
endif()

add_library(platform STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(platform PROPERTIES CXX_STANDARD 11)
target_link_libraries(platform engine glm ${libraries})
target_compile_definitions(platform PUBLIC ${defines})
