
include_directories(src lib/dyncall)
LINK_DIRECTORIES(lib)

file(GLOB_RECURSE runtime_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(runtime STATIC ${runtime_source_files} )
