cmake_minimum_required(VERSION 3.6)
project(logic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -Wsign-conversion -Wextra")
set(SRC_DIR "src")
set(INC_DIR "inc")
set(SRC_EXT "cpp")
set(INC_EXT "hpp")
set(TARGET "logic")
set(TEST_DIR "test")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Main binary

file(GLOB_RECURSE SOURCE_FILES ${SRC_DIR}/*.${SRC_EXT} ${INC_DIR}/*.${INC_EXT})
include_directories(${INC_DIR})
add_executable(${TARGET} ${SOURCE_FILES})

# Test binary

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${INC_DIR})
file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.${SRC_EXT} ${INC_DIR}/*.${INC_EXT})
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests Catch)
enable_testing()
add_test(NAME LogicTests COMMAND run_tests)