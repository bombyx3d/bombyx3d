
cmake_minimum_required(VERSION 3.1)
project(glm)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB detail_files glm/detail/*)
file(GLOB gtc_files glm/gtc/*)
file(GLOB gtx_files glm/gtx/*)
file(GLOB root_files glm/*.hpp)

add_library(glm STATIC ${EXCLUDE_FROM_ALL}
    ${detail_files}
    ${gtc_files}
    ${gtx_files}
    ${root_files}
)

source_group("Source Files\\detail" FILES ${detail_files})
source_group("Source Files\\gtc" FILES ${gtc_files})
source_group("Source Files\\gtx" FILES ${gtx_files})
source_group("Source Files" FILES ${root_files})

target_include_directories(glm BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
