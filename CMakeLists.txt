cmake_minimum_required(VERSION 3.20)
project(ai-problem-solver CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Redirect output executables and libraries to the root /bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include the header files directory (.hpp)
include_directories(include)

# Register the core framework source files (.cpp)
set(FRAMEWORK_SOURCES
    src/Node.cpp
    src/State.cpp
    src/Solver.cpp
)

# Demo executable for framework testing
add_executable(8_puzzle_demo 
    tests/8_puzzle_main.cpp 
    ${FRAMEWORK_SOURCES}
)

# Compilation flags for capturing warnings and errors
if(MSVC)
    target_compile_options(8_puzzle_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(8_puzzle_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()