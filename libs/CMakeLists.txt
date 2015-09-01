
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(/wd4047 /wd4244 /wd4267 /wd4305 /wd4456 /wd4458)
else()
    add_definitions(-w)
endif()

add_subdirectory(glm)
