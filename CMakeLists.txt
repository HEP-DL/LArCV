cmake_minimum_required (VERSION 2.6)
project (larcv)

#setup  ROOT dependency
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
find_package(larlite REQUIRED)

add_subdirectory(core)
add_subdirectory(app)

add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/python ${CMAKE_INSTALL_PREFIX}/python
    DEPENDS ${LarLiteAnalysis})

install(EXPORT larcvConfig DESTINATION share/larcv/cmake)