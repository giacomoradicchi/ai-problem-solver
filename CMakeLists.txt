cmake_minimum_required(VERSION 3.20)
project(ai-problem-solver CXX)

# Forza lo standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include la cartella degli header file (.hpp)
include_directories(include)

# Registra i sorgenti del framework (.cpp)
set(FRAMEWORK_SOURCES
    src/Node.cpp
)

# Eseguibile di esempio per testare il framework
add_executable(8_puzzle_demo 
    tests/8_puzzle_main.cpp 
    ${FRAMEWORK_SOURCES}
)

# Flag di compilazione per catturare warning ed errori
if(MSVC)
    target_compile_options(8_puzzle_demo PRIVATE /W4 /permissive-)
else()
    target_compile_options(8_puzzle_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()