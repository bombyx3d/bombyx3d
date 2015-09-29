
project(jpeg)
include(../../cmake/DisableWarnings.cmake)

configure_file(jconfig.vc "${CMAKE_CURRENT_BINARY_DIR}/jconfig.h" COPYONLY)

set(source_files
    cderror.h
    cdjpeg.c
    cdjpeg.h
    jaricom.c
    jcapimin.c
    jcapistd.c
    jcarith.c
    jccoefct.c
    jccolor.c
    jcdctmgr.c
    jchuff.c
    jcinit.c
    jcmainct.c
    jcmarker.c
    jcmaster.c
    jcomapi.c
    jconfig.vc
    jcparam.c
    jcprepct.c
    jcsample.c
    jctrans.c
    jdapimin.c
    jdapistd.c
    jdarith.c
    jdatadst.c
    jdatasrc.c
    jdcoefct.c
    jdcolor.c
    jdct.h
    jddctmgr.c
    jdhuff.c
    jdinput.c
    jdmainct.c
    jdmarker.c
    jdmaster.c
    jdmerge.c
    jdpostct.c
    jdsample.c
    jdtrans.c
    jerror.c
    jerror.h
    jfdctflt.c
    jfdctfst.c
    jfdctint.c
    jidctflt.c
    jidctfst.c
    jidctint.c
    jinclude.h
    jmemmgr.c
    jmemnobs.c
    jmemsys.h
    jmorecfg.h
    jpegint.h
    jpeglib.h
    jquant1.c
    jquant2.c
    jutils.c
    jversion.h
    rdbmp.c
    rdcolmap.c
    rdgif.c
    rdppm.c
    rdswitch.c
    rdtarga.c
    transupp.c
    transupp.h
    wrbmp.c
    wrgif.c
    wrppm.c
    wrtarga.c
)

z_add_library(jpeglib STATIC ${EXCLUDE_FROM_ALL} ${source_files})

source_group("Source Files" FILES ${source_files})
set_source_files_properties(jconfig.vc PROPERTIES HEADER_FILE_ONLY TRUE)

set_target_properties("${jpeglib}" PROPERTIES OUTPUT_NAME "jpeg")
target_include_directories("${jpeglib}" BEFORE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories("${jpeglib}" BEFORE PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
