
macro(set_source_groups)
    foreach(file ${ARGN})
        get_filename_component(path "${file}" DIRECTORY)
        string(REPLACE "/" "\\" path "${path}")
        source_group("Source Files\\${path}" FILES "${file}")
    endforeach()
endmacro()
