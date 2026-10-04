#ifndef PUZZLE_PROBLEM_HPP
#define PUZZLE_PROBLEM_HPP

#include "Problem.hpp"
#include "PuzzleState.hpp"
#include "Node.hpp"
#include <memory>
#include <vector>

class PuzzleProblem : public Problem {
    private:
        std::shared_ptr<PuzzleState> initial_state;
        std::shared_ptr<PuzzleState> goal_state;

        // converts a generic State into a PuzzleState (throws if it's not a PuzzleState)
        [[nodiscard]] static const PuzzleState& to_puzzle_state(const std::shared_ptr<State>& state);

    public:
        // creates the problem from an initial state. the goal is the standard one: 1, 2, ..., N and the empty tile as the last cell
        explicit PuzzleProblem(std::shared_ptr<PuzzleState> initial_state);

        [[nodiscard]] std::shared_ptr<PuzzleState> get_goal_state() const {
            return goal_state;
        }

        // virtual interface overrides from base class Problem
        [[nodiscard]] std::shared_ptr<State> initial() const override {
            return initial_state;
        }

        [[nodiscard]] bool is_goal_state(const std::shared_ptr<State>& state) const override {
            return *state == *goal_state;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Action>> actions(const std::shared_ptr<State>& state) const override;

        [[nodiscard]] std::shared_ptr<State> result(const std::shared_ptr<State>& state, const std::shared_ptr<Action>& action) const override;

        // every move costs 1 (parameters without name since they're not used)
        [[nodiscard]] double step_cost(const std::shared_ptr<State>&, const std::shared_ptr<Action>&, const std::shared_ptr<State>&) const override {
            return 1.0;
        }

        // heuristics for A* (static, so they can be passed directly to the solver)

        // number of tiles not in their goal position (the empty tile is not counted)
        [[nodiscard]] static double misplaced_tiles(const std::shared_ptr<Node>& node);

        // sum of the distances (rows + cols) of every tile from its goal position (the empty tile is not counted)
        [[nodiscard]] static double manhattan_distance(const std::shared_ptr<Node>& node);
};

#endif
