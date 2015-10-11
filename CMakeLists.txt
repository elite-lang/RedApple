cmake_minimum_required(VERSION 2.8)
project(RedApple)

set(LLVM_TARGETS_TO_BUILD X86)
set(LLVM_BUILD_RUNTIME OFF)
set(LLVM_BUILD_TOOLS OFF)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(BISON)
find_package(FLEX)

SET (CMAKE_CXX_COMPILER_ENV_VAR "clang++")
SET (CMAKE_CXX_FLAGS "-std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL  "-Os -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE  "-O4 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

FLEX_TARGET(MyScanner ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_lex.l
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_lex.cpp COMPILE_FLAGS -w)
BISON_TARGET(MyParser ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_parser.y
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(Debug Release build include src src/Model)

file(GLOB_RECURSE source_files	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp)
add_executable(redapple ${source_files} 
						${BISON_MyParser_OUTPUTS} 
						${FLEX_MyScanner_OUTPUTS})

install(TARGETS redapple RUNTIME DESTINATION bin)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs 
	support core irreader executionengine interpreter 
	mc mcjit bitwriter x86codegen target)

# Link against LLVM libraries
target_link_libraries(redapple ${llvm_libs})