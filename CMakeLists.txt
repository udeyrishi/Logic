cmake_minimum_required(VERSION 3.6)
project(logic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SRC_DIR "src")
set(INC_DIR "inc")
set(SRC_EXT "cpp")
set(INC_EXT "hpp")
set(TARGET "logic")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.${SRC_EXT} ${INC_DIR}/*.${INC_EXT})
include_directories(${INC_DIR})
add_executable(${TARGET} ${SOURCE_FILES})