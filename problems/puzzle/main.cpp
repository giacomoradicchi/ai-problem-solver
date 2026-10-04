#include "PuzzleProblem.hpp"
#include "Solver.hpp"
#include <iostream>
#include <thread>
#include <chrono>

#define FRAME_DELAY_MS 500 // time between two frames of the animation (in milliseconds)

// prints the number of moves and the list of actions from the root to the solution node
void print_solution(const std::string& algorithm, const std::shared_ptr<Node>& solution) {
    std::cout << algorithm << ": ";

    if (solution == nullptr) {
        std::cout << "no solution found" << std::endl;
        return;
    }

    // going back from the solution to the root, so the actions are in reverse order
    std::vector<std::string> actions;
    for (std::shared_ptr<Node> node = solution; node->get_parent() != nullptr; node = node->get_parent()) {
        actions.push_back(node->get_action()->to_string());
    }

    std::cout << solution->get_depth() << " moves ->";
    for (int i = actions.size() - 1; i >= 0; i--) {
        std::cout << " " << actions[i];
    }
    std::cout << std::endl;
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

    std::shared_ptr<Node> bfs_solution = solver.breadth_first_search(problem);
    std::shared_ptr<Node> misplaced_solution = solver.a_star(problem, PuzzleProblem::misplaced_tiles);
    std::shared_ptr<Node> manhattan_solution = solver.a_star(problem, PuzzleProblem::manhattan_distance);

    animate_solution(manhattan_solution);

    // summary under the last frame (the goal state)
    std::cout << std::endl;
    print_solution("BFS", bfs_solution);
    print_solution("A* (misplaced tiles)", misplaced_solution);
    print_solution("A* (manhattan distance)", manhattan_solution);

    return 0;
}
