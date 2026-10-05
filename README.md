# AI Problem Solver

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=cplusplus)
[![AIMA Architecture](https://img.shields.io/badge/Architecture-AIMA%204th%20Ed.-orange.svg?style=flat-square)](http://aima.cs.berkeley.edu)
[![AIMA GitHub](https://img.shields.io/badge/Reference-aimacode-black.svg?style=flat-square&logo=github)](https://github.com/aimacode)
![License](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)

An object-oriented, highly performant C++20 framework for solving classical AI state-space problems using state-of-the-art search algorithms. Built upon the architectural principles of **Russell & Norvig's AIMA** (*[Artificial Intelligence: A Modern Approach](http://aima.cs.berkeley.edu)*) and aligned with the implementations in [aimacode](https://github.com/aimacode).

---

## References & Inspiration

- **Official Textbook**: [AIMA - Artificial Intelligence: A Modern Approach (4th Edition)](http://aima.cs.berkeley.edu)
- **Official Repository**: [aimacode on GitHub](https://github.com/aimacode) — Reference code repositories in Python, Java, and other languages.

---

## Key Features

- **Modern C++20 Design**: Full move semantics, zero-cost abstractions, `[[nodiscard]]`, and explicit resource management with smart pointers (`std::shared_ptr`).
- **Graph & Tree Search Modes**: Built-in $O(1)$ hash-based state duplicate detection (`Reached` set/map) for graph search, prevent memory leaks and infinite loops.
- **Uninformed & Informed Algorithms**:
  - **Uninformed**: Breadth-First Search (BFS), Depth-First Search (DFS), Iterative Deepening (IDS), Uniform-Cost Search (UCS).
  - **Informed / Heuristic**: Best-First Search, A*, Iterative Deepening A* (IDA*), and Bidirectional Search framework.
- **Type-Safe Domain Abstraction**: Decoupled `State`, `Action`, and `Problem` interfaces allow easy definition of custom domains (e.g., 8-Puzzle, Vacuum World, Grid Navigation).

---

## Getting Started

### Requirements

- [CMake](https://cmake.org/download/) 3.20 or newer
- A C++20 compiler (GCC 10+, Clang 10+, or MSVC from Visual Studio 2019 16.10+)

### Download

From the folder in which you want to download the repository:

```bash
git clone https://github.com/giacomoradicchi/ai-problem-solver.git
```

Or download it using [download-directory.github.io](https://download-directory.github.io) and paste `https://github.com/giacomoradicchi/ai-problem-solver.git`.

### Build & Run on Linux / macOS

```bash
cd ai-problem-solver
mkdir build && cd build
cmake ..
make
cd .. && ./bin/puzzle_demo
```

### Build & Run on Windows

**Option A: Visual Studio (MSVC)**

Open the *Developer PowerShell for Visual Studio* (or any terminal where `cmake` is available) and run:

```powershell
cd ai-problem-solver
mkdir build; cd build
cmake ..
cmake --build . --config Release
cd ..; .\bin\Release\puzzle_demo.exe
```

> Visual Studio is a multi-configuration generator, so the executable is placed in `bin\Release\` (or `bin\Debug\` if you build with `--config Debug`).

**Option B: MinGW-w64 (GCC)**

Make sure `g++`, `mingw32-make` and `cmake` are on your `PATH`, then run:

```powershell
cd ai-problem-solver
mkdir build; cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
cd ..; .\bin\puzzle_demo.exe
```

---

## Architecture Diagram

```mermaid
classDiagram
    direction TB

    class State {
        <<interface>>
    }

    class Action {
        <<interface>>
        +to_string(): String
    }

    class Problem {
        <<interface>>
        +initial(): State
        +is_goal_state(state: State): Boolean
        +actions(state: State): List~Action~
        +result(state: State, action: Action): State
        +step_cost(state: State, action: Action, next_state: State): Real
        +path_cost(c: Real, state: State, action: Action, next_state: State): Real
    }

    class Node {
        -state: State
        -parent: Node
        -action: Action
        -path_cost: Real
        -depth: Integer
        +expand(problem: Problem): List~Node~
        +child_node(problem: Problem, action: Action): Node
    }

    class Solver {
        <<interface>>
        +breadth_first_search(problem: Problem, graph_search: Boolean): Node
        +depth_first_search(problem: Problem, graph_search: Boolean): Node
        +iterative_deepening(problem: Problem, graph_search: Boolean): Node
        +uniform_cost_search(problem: Problem, graph_search: Boolean): Node
        +a_star(problem: Problem, graph_search: Boolean): Node
        +ida_star(problem: Problem, graph_search: Boolean): Node
        +best_first_search(problem: Problem, f: Function, graph_search: Boolean): Node
        +bidirectional(problem: Problem, f_forward: Function, f_backwards: Function, graph_search_forward: Boolean, graph_search_backwards: Boolean): Node
    }

    State "1" -- "0..*" Problem : handles
    Problem "1" -- "0..*" Action : defines

    State "1" -- "0..*" Node : represents
    Node "0..*" -- "0..1" Action : generated by

    Node "0..*" -- "0..1" Node : parent
    Problem "1" -- "1" Solver : queries
    Solver "1" -- "0..1" Node : returns
```