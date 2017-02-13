file(GLOB HEADERS .  *.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
list(REMOVE_ITEM HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h)
file(GLOB SOURCES . *.cxx )

ROOT_GENERATE_DICTIONARY(G__CVUtil ${HEADERS} LINKDEF LinkDef.h)

add_library(LarCVCVUtil SHARED ${SOURCES} )#G__CVUtil.cxx 
target_link_libraries(LarCVCVUtil ${ROOT_LIBRARIES})
install(TARGETS LarCVCVUtil DESTINATION lib EXPORT larcvConfig)
install(FILES ${HEADERS} DESTINATION include )
export(TARGETS LarCVCVUtil FILE larcvConfig.cmake)

#file(GLOB applications bin/*.cc)
#foreach(_app ${applications})
#  get_filename_component (name_without_extension ${_app} NAME_WE)
#  add_executable(${name_without_extension} ${_app} )
#  target_link_libraries(${name_without_extension} LarCVCVUtil)
#  install(TARGETS ${name_without_extension} DESTINATION bin)
#endforeach()