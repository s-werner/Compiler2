cmake_minimum_required(VERSION 3.10)
project(MyCompiler)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Main Compiler Executable
add_executable(MyCompiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/interpreter.cpp
    src/symboltable.cpp
)

# Enable testing
enable_testing()

# Include FetchContent module to download Google Test
include(FetchContent)

# Fetch Google Test
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# Make available
FetchContent_MakeAvailable(googletest)

# Test utility files
set(TEST_UTILS tests/TestUtils.cpp)

# Add test executables
file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(runTests ${TEST_SOURCES} src/lexer.cpp src/parser.cpp src/ast.cpp src/interpreter.cpp src/symboltable.cpp ${TEST_UTILS})

# Link Google Test libraries
target_link_libraries(runTests gtest gtest_main)

# Add the tests to CTest
include(GoogleTest)
gtest_discover_tests(runTests)
