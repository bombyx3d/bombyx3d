
file(GLOB source_files *.cpp *.h *.hpp)
source_group("Source Files" FILES ${source_files})

add_executable(assimp EXCLUDE_FROM_ALL ${source_files})
target_link_libraries(assimp libassimp)
