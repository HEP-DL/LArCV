file(GLOB HEADERS .  *.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
list(REMOVE_ITEM HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h)
file(GLOB SOURCES . *.cxx )

ROOT_GENERATE_DICTIONARY(G__CPPUtil ${HEADERS} LINKDEF LinkDef.h)

add_library(LarCVCPPUtil SHARED ${SOURCES} )#G__CPPUtil.cxx 
target_link_libraries(LarCVCPPUtil ${ROOT_LIBRARIES})
install(TARGETS LarCVCPPUtil DESTINATION lib EXPORT larcvConfig)
install(FILES ${HEADERS} DESTINATION include )
export(TARGETS LarCVCPPUtil FILE larcvConfig.cmake)

#file(GLOB applications bin/*.cc)
#foreach(_app ${applications})
#  get_filename_component (name_without_extension ${_app} NAME_WE)
#  add_executable(${name_without_extension} ${_app} )
#  target_link_libraries(${name_without_extension} LarCVCPPUtil)
#  install(TARGETS ${name_without_extension} DESTINATION bin)
#endforeach()