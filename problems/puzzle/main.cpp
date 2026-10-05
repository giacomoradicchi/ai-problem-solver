#include "PuzzleProblem.hpp"
#include "Solver.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <algorithm>

// NOTE: the "graphic" implementation (animation on terminal) was created by LLM.

#define FRAME_DELAY_MS 500 // time between two frames of the animation (in milliseconds)

// summary table layout
#define SUMMARY_WIDTH 72        // total width of the table (in characters)
#define NAME_COLUMN_WIDTH 40    // width of the algorithm column
#define MOVES_COLUMN_WIDTH 8    // width of the moves column
#define ACTIONS_PER_LINE 10     // actions printed on each line
#define MAX_ACTIONS_SHOWN 50    // longer solutions (e.g. DFS) are truncated

// prints the title and the column names of the summary table
void print_summary_header() {
    const std::string title = "SOLUTIONS SUMMARY";

    std::cout << std::string(SUMMARY_WIDTH, '=') << "\n"
              << std::string((SUMMARY_WIDTH - title.size()) / 2, ' ') << title << "\n"
              << std::string(SUMMARY_WIDTH, '=') << "\n"
              << "  " << std::left << std::setw(NAME_COLUMN_WIDTH) << "Algorithm"
              << std::right << std::setw(MOVES_COLUMN_WIDTH) << "Moves" << "\n"
              << std::string(SUMMARY_WIDTH, '-') << std::endl;
}

// prints a row of the summary table: the algorithm, the number of moves and the list of actions from the root to the solution node
void print_solution(const std::string& algorithm, const std::shared_ptr<Node>& solution) {
    std::cout << "  " << std::left << std::setw(NAME_COLUMN_WIDTH) << algorithm << std::right;

    if (solution == nullptr) {
        std::cout << std::setw(MOVES_COLUMN_WIDTH) << "-" << "   (no solution found)\n"
                  << std::string(SUMMARY_WIDTH, '-') << std::endl;
        return;
    }

    std::cout << std::setw(MOVES_COLUMN_WIDTH) << solution->get_depth() << "\n";

    // going back from the solution to the root, so the actions are in reverse order
    std::vector<std::string> actions;
    for (std::shared_ptr<Node> node = solution; node->get_parent() != nullptr; node = node->get_parent()) {
        actions.push_back(node->get_action()->to_string());
    }

    // actions in lines of ACTIONS_PER_LINE, each one starting with the number of its first move
    std::size_t shown = std::min<std::size_t>(actions.size(), MAX_ACTIONS_SHOWN);
    for (std::size_t i = 0; i < shown; i++) {
        if (i % ACTIONS_PER_LINE == 0) {
            std::cout << (i == 0 ? "" : "\n") << "    " << std::setw(5) << i + 1 << " |";
        }
        std::cout << "  " << actions[actions.size() - 1 - i];
    }
    if (actions.size() > shown) {
        std::cout << "\n          |  ... (" << actions.size() - shown << " more)";
    }
    if (shown > 0) {
        std::cout << "\n";
    }

    std::cout << std::string(SUMMARY_WIDTH, '-') << std::endl;
}

// erases the last lines printed on the terminal (ANSI escape codes)
void erase_lines(int lines) {
    if (lines == 0) {
        return;
    }

    std::cout << "\033[" << lines << "A"; // moves the cursor up by the number of lines
    std::cout << "\033[J";                // erases everything from the cursor to the end of the screen
}

// shows the solution as an animation: one frame for every state, from the initial state to the goal
void animate_solution(const std::shared_ptr<Node>& solution) {
    if (solution == nullptr) {
        std::cout << "no solution to animate" << std::endl;
        return;
    }

    // going back from the solution to the root, so the nodes are in reverse order
    std::vector<std::shared_ptr<Node>> path;
    for (std::shared_ptr<Node> node = solution; node != nullptr; node = node->get_parent()) {
        path.push_back(node);
    }

    unsigned int total_moves = solution->get_depth();
    int previous_frame_lines = 0; // lines of the previous frame (to erase it before printing the next one)

    for (int i = path.size() - 1; i >= 0; i--) {
        std::shared_ptr<Node> node = path[i];

        // building the whole frame first, so we can count its lines
        std::string frame;
        if (node->get_parent() == nullptr) {
            frame = "initial state (0/" + std::to_string(total_moves) + ")\n";
        } else {
            frame = "move " + std::to_string(node->get_depth()) + "/" + std::to_string(total_moves) + ": " + node->get_action()->to_string() + "\n";
        }
        frame += node->get_state()->to_string();

        erase_lines(previous_frame_lines);
        std::cout << frame << std::flush; // flush, so the frame is printed before waiting

        // counting the lines of this frame (one for every "\n")
        previous_frame_lines = 0;
        for (char c : frame) {
            if (c == '\n') {
                previous_frame_lines++;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY_MS));
    }
}

int main() {
    // initial state of the 8-puzzle (0 is the empty tile)
    std::vector<std::uint8_t> board = {
        7, 2, 4,
        5, 0, 6,
        8, 3, 1
    };

    auto initial_state = std::make_shared<PuzzleState>(8, board);
    auto problem = std::make_shared<PuzzleProblem>(initial_state);

    Solver solver;

    std::shared_ptr<Node> dfs_solution = solver.depth_first_search(problem, true);
    std::shared_ptr<Node> bfs_solution = solver.breadth_first_search(problem);
    std::shared_ptr<Node> ucs_solution = solver.uniform_cost_search(problem);
    std::shared_ptr<Node> iterative_deepening_solution = solver.iterative_deepening(problem);
    std::shared_ptr<Node> greedy_manhattan_solution = solver.best_first_search(problem, PuzzleProblem::manhattan_distance);
    std::shared_ptr<Node> misplaced_solution = solver.a_star(problem, PuzzleProblem::misplaced_tiles);
    std::shared_ptr<Node> manhattan_solution = solver.a_star(problem, PuzzleProblem::manhattan_distance);
    std::shared_ptr<Node> weighted_manhattan_solution = solver.a_star(problem, PuzzleProblem::manhattan_distance, true, 2);

    animate_solution(manhattan_solution);

    // summary under the last frame (the goal state)
    std::cout << std::endl;
    print_summary_header();
    print_solution("DFS", dfs_solution);
    print_solution("BFS", bfs_solution);
    print_solution("UCS", ucs_solution);
    print_solution("Iterative-Deepening", iterative_deepening_solution);
    print_solution("Greedy-Search (manhattan distance)", greedy_manhattan_solution);
    print_solution("A* (misplaced tiles)", misplaced_solution);
    print_solution("A* (manhattan distance)", manhattan_solution);
    print_solution("Weighted-A* (manhattan distance)", weighted_manhattan_solution);

    return 0;
}
