
include_directories(src lib/dyncall)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../runtime )
file(GLOB_RECURSE runtime_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(runtime STATIC ${runtime_source_files} )
target_link_libraries(runtime dyncall_s)
install(TARGETS runtime ARCHIVE DESTINATION runtime)
