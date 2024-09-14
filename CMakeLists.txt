cmake_minimum_required(VERSION 3.10)
project(MyCompiler)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include the directory for header files
include_directories(include)

# Define the main executable
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

# Fetch and make available
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(runTests
    tests/tests.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/interpreter.cpp
    src/symboltable.cpp
)

# Link Google Test libraries
target_link_libraries(runTests gtest gtest_main)

# Add the tests to CMake
add_test(NAME MyCompilerTests COMMAND runTests)
