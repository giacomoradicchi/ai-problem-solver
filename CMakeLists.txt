cmake_minimum_required(VERSION 3.20)
project(AIMA_Search_Framework CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warning flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Output directory for compiled binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ------------------------------------------------------------------------------
# 1. FRAMEWORK CORE LIBRARY (Generic AI Search Engine)
# ------------------------------------------------------------------------------
add_library(search_framework STATIC
    framework/src/Node.cpp
    framework/src/Solver.cpp
)

target_include_directories(search_framework PUBLIC
    framework/include
)

# ------------------------------------------------------------------------------
# 2. CONCRETE PROBLEMS (Problem Domains)
# ------------------------------------------------------------------------------

# 8-Puzzle Domain Executable
add_executable(puzzle_demo
    problems/puzzle/src/PuzzleState.cpp
    problems/puzzle/src/PuzzleProblem.cpp
    problems/puzzle/main.cpp
)

target_include_directories(puzzle_demo PRIVATE
    problems/puzzle/include
)

# Link the core search framework to the 8-puzzle target
target_link_libraries(puzzle_demo PRIVATE search_framework)