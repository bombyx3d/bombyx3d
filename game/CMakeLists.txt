
set(source_files
    client/Application.cpp
    client/Application.h
)

add_library(game STATIC EXCLUDE_FROM_ALL ${source_files})
set_source_groups(${source_files})

set_target_properties(game PROPERTIES CXX_STANDARD 11)
target_link_libraries(game engine glm)
