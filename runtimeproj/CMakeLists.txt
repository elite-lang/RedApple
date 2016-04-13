SET (CMAKE_CXX_FLAGS "-std=c++11")
include_directories(src lib/dyncall)
file(GLOB_RECURSE runtime_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(runtime STATIC ${runtime_source_files} )
install(TARGETS runtime ARCHIVE DESTINATION runtime)
