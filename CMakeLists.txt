cmake_minimum_required(VERSION 3.2)
project(logic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wextra -Werror -Wsign-conversion -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Werror -Wsign-conversion -pedantic")

set(SRC_DIR "src")
set(INC_DIR "inc")
set(SRC_EXT "cpp")
set(INC_EXT "hpp")
set(TARGET "logic")
set(TEST_DIR "test")

# Needed to prevent collision b/w Main.cpp#main() vs. catch lib's main()
set(TARGET_MAIN_CPP "${SRC_DIR}/Main.cpp")

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
file(GLOB_RECURSE TEST_SOURCES ${SRC_DIR}/*.${SRC_EXT} ${TEST_DIR}/*.${SRC_EXT} ${INC_DIR}/*.${INC_EXT})

# Remove the main Main.cpp from test binary
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_MAIN_CPP})

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests Catch)
enable_testing()
add_test(NAME LogicTests COMMAND run_tests)