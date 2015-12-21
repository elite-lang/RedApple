cmake_minimum_required(VERSION 2.8)
project(RedApple)

if(NOT BUILD_ALL)
	
SET (CMAKE_BUILD_TYPE Debug) # 默认构建Debug模式
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

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

FLEX_TARGET(MyScanner ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_lex.l
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_lex.cpp COMPILE_FLAGS -w)
BISON_TARGET(MyParser ${CMAKE_CURRENT_SOURCE_DIR}/src/redapple_parser.y
					  ${CMAKE_CURRENT_BINARY_DIR}/redapple_parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

endif()

if(MSVC)
	#define llvm version
	if (${LLVM_PACKAGE_VERSION} LESS 3.7.0)
		set(CMAKE_CXX_FLAGS "/DLLVM_3_6")
	else()
		set(CMAKE_CXX_FLAGS "/DLLVM_3_7")
	endif()
endif()



# 核心路径配置
include_directories(includes includes/Model includes/LLCG src src/Utils src/MetaModel src/cJSON 
    src/LLCG/ src/LLCG/LLVMLIB)

file(GLOB_RECURSE source_files	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/Model/*.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/Macro/*.cpp
								${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/*.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/MetaModel/*.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/LLCG/*.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/LLCG/*/*.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/cJSON/*.c)
list(REMOVE_ITEM  source_files  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

if(USE_DYNAMIC)
	add_library(red SHARED ${source_files}) # 使用动态库
else()
	add_library(red STATIC ${source_files}) # 使用静态库
endif()

add_subdirectory(runtime)








if(NOT BUILD_ALL)

add_executable(redapple ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})

install(TARGETS redapple RUNTIME DESTINATION bin)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs
	support core irreader executionengine interpreter
	mc mcjit bitwriter x86codegen target)

# Link against LLVM libraries
target_link_libraries(redapple red ${llvm_libs})

endif()
