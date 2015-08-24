cmake_minimum_required(VERSION 2.8)
project(RedApple)
SET(CMAKE_CXX_COMPILER_ENV_VAR "clang++")
SET(CMAKE_CXX_FLAGS "-std=c++11")

find_package(BISON)
find_package(FLEX)
FLEX_TARGET(MyScanner ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_lex.l
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_lex.cpp COMPILE_FLAGS -w)
BISON_TARGET(MyParser ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_parser.y
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(build src src/Model)

file(GLOB_RECURSE source_files	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp)
add_executable(redapple ${source_files} 
						${BISON_MyParser_OUTPUTS} 
						${FLEX_MyScanner_OUTPUTS})
install(TARGETS redapple RUNTIME DESTINATION bin)
