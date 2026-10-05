#include "PuzzleProblem.hpp"
#include <stdexcept>
#include <string>
#include <cstdlib>

PuzzleProblem::PuzzleProblem(std::shared_ptr<PuzzleState> initial_state) {
    this->initial_state = initial_state;
    goal_state = std::make_shared<PuzzleState>(initial_state->get_num_tiles()); // goal state with the same number of tiles
}

const PuzzleState& PuzzleProblem::to_puzzle_state(const std::shared_ptr<State>& state) {
    const PuzzleState* puzzle = dynamic_cast<const PuzzleState*>(state.get()); // nullptr if state is not a PuzzleState
    if (puzzle == nullptr) {
        throw std::invalid_argument("the state is not a PuzzleState");
    }

    return *puzzle;
}

std::vector<std::shared_ptr<Action>> PuzzleProblem::actions(const std::shared_ptr<State>& state) const {
    const PuzzleState& puzzle = to_puzzle_state(state);

    std::size_t row = puzzle.get_zero_pos().first;
    std::size_t col = puzzle.get_zero_pos().second;
    std::size_t last = puzzle.get_side_length() - 1;

    // the actions move the empty tile (0), so they depend only on where the empty tile is
    std::vector<std::shared_ptr<Action>> possible_actions;

    if (row > 0) {
        possible_actions.push_back(std::make_shared<Action>("↑"));
    }
    if (row < last) {
        possible_actions.push_back(std::make_shared<Action>("↓"));
    }
    if (col > 0) {
        possible_actions.push_back(std::make_shared<Action>("←"));
    }
    if (col < last) {
        possible_actions.push_back(std::make_shared<Action>("→"));
    }

    return possible_actions;
}

std::shared_ptr<State> PuzzleProblem::result(const std::shared_ptr<State>& state, const std::shared_ptr<Action>& action) const {
    const PuzzleState& puzzle = to_puzzle_state(state);

    std::size_t row = puzzle.get_zero_pos().first;
    std::size_t col = puzzle.get_zero_pos().second;

    // position where the empty tile is going to be after the move
    std::size_t new_row = row;
    std::size_t new_col = col;

    const std::string& name = action->to_string();
    if (name == "↑") {
        new_row--;
    } else if (name == "↓") {
        new_row++;
    } else if (name == "←") {
        new_col--;
    } else if (name == "→") {
        new_col++;
    } else {
        throw std::invalid_argument("unknown action: " + name);
    }

    // if the empty tile is in row (or col) 0, 0 - 1 becomes a huge number (size_t is unsigned), so this check covers also that case
    if (new_row >= puzzle.get_side_length() || new_col >= puzzle.get_side_length()) {
        throw std::invalid_argument("action " + name + " moves the empty tile out of the board");
    }

    // new state: copy of the current one with the empty tile moved (the current state is not modified)
    auto next_state = std::make_shared<PuzzleState>(puzzle);
    next_state->swap_tiles(row, col, new_row, new_col);

    return next_state;
}

double PuzzleProblem::misplaced_tiles(const std::shared_ptr<Node>& node) {
    const PuzzleState& puzzle = to_puzzle_state(node->get_state());
    std::size_t side = puzzle.get_side_length();

    int misplaced = 0;
    for (std::size_t row = 0; row < side; row++) {
        for (std::size_t col = 0; col < side; col++) {
            std::size_t tile = puzzle(row, col);

            // in the goal state, the cell (row, col) contains the tile row * side + col + 1
            if (tile != 0 && tile != row * side + col + 1) {
                misplaced++;
            }
        }
    }

    return misplaced;
}

double PuzzleProblem::manhattan_distance(const std::shared_ptr<Node>& node) {
    const PuzzleState& puzzle = to_puzzle_state(node->get_state());
    std::size_t side = puzzle.get_side_length();

    int distance = 0;
    for (std::size_t row = 0; row < side; row++) {
        for (std::size_t col = 0; col < side; col++) {
            std::size_t tile = puzzle(row, col);

            if (tile == 0) {
                continue; // the empty tile is not counted (otherwise the heuristic is not admissible)
            }

            // goal position of the tile (inverse of row * side + col + 1)
            std::size_t goal_row = (tile - 1) / side;
            std::size_t goal_col = (tile - 1) % side;

            // cast to int, otherwise the difference between size_t (unsigned) can't be negative
            distance += std::abs(static_cast<int>(row) - static_cast<int>(goal_row));
            distance += std::abs(static_cast<int>(col) - static_cast<int>(goal_col));
        }
    }

    return distance;
}
