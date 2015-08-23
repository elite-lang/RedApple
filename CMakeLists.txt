cmake_minimum_required(VERSION 2.8)
project(RedApple)
SET(CMAKE_CXX_COMPILER_ENV_VAR "CXX")
SET(CMAKE_CXX_FLAGS "-std=c++11")

find_package(BISON)
find_package(FLEX)
BISON_TARGET(MyParser ${CMAKE_CURRENT_BINARY_DIR}/src/redapple_parser.y 
					  ${CMAKE_CURRENT_BINARY_DIR}/src/redapple_parser.cpp)
FLEX_TARGET(MyScanner redapple_lex.l  ${CMAKE_CURRENT_BINARY_DIR}/src/redapple_lex.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

file(GLOB_RECURSE source_files	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
								"${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp")
add_executable(redapple ${source_files})
install(TARGETS redapple RUNTIME DESTINATION bin)
