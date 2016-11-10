cmake_minimum_required(VERSION 3.2)
project(logic)

# Build flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
# Don't use -O3, as it'll turn assert() to no-ops
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Wextra -Werror -Wsign-conversion -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -Werror -Wsign-conversion -pedantic")

# Output vars
set(CORE_LIB "logic_core")
set(LANG_LIB "logic_lang")
set(TARGET "logic")
set(TEST_LIB "catch")
set(TEST_TARGET "logic_tests")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Input vars
set(SRC_DIR "src")
set(INC_DIR "inc")
set(TEST_DIR "test")
set(SRC_EXT "cpp")
set(INC_EXT "hpp")
set(MAIN_CPP "Main.cpp")
set(CORE_MODULE "core")
set(LANG_MODULE "lang")

# The core library
file(GLOB_RECURSE CORE_LIB_SOURCES ${SRC_DIR}/${CORE_MODULE}/*.${SRC_EXT})
add_library(${CORE_LIB} STATIC ${CORE_LIB_SOURCES})

# The lang library
file(GLOB_RECURSE LANG_LIB_SOURCES ${SRC_DIR}/${LANG_MODULE}/*.${SRC_EXT})
add_library(${LANG_LIB} STATIC ${LANG_LIB_SOURCES})
# lang lib depends on core
target_link_libraries(${LANG_LIB} ${CORE_LIB})

# The main binary
add_executable(${TARGET} ${SRC_DIR}/${MAIN_CPP})
include_directories(${INC_DIR})
target_link_libraries(${TARGET} ${LANG_LIB})

# The Catch testing library
add_library(${TEST_LIB} INTERFACE)
target_include_directories(${TEST_LIB} INTERFACE ${TEST_DIR}/${INC_DIR})

# The test binary
file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.${SRC_EXT})
add_executable(${TEST_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET} ${TEST_LIB})
target_link_libraries(${TEST_TARGET} ${LANG_LIB})

# Expose tests to CMake
enable_testing()
add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
