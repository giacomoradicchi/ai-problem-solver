#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <memory>
#include <vector>
#include "State.hpp"
#include "Action.hpp"


class Problem {
    public:
        virtual ~Problem() = default;
        
        // Returns the initial state of the problem
        [[nodiscard]] virtual std::shared_ptr<State> initial() const = 0;

        // Checks whether the state is the goal
        [[nodiscard]] virtual bool is_goal_state(const std::shared_ptr<State>& state) const = 0;

        // Returns all the admissible actions from the state
        [[nodiscard]] virtual std::vector<std::shared_ptr<Action>> actions(const std::shared_ptr<State>& state) const = 0;

        // Returns the next state reached from state by a certain action (Transition Model)
        [[nodiscard]] virtual std::shared_ptr<State> result(const std::shared_ptr<State>& state, const std::shared_ptr<Action>& action) const = 0;

        // Returns the cost of a single step 
        [[nodiscard]] virtual double step_cost(const std::shared_ptr<State>& state, const std::shared_ptr<Action>& action, const std::shared_ptr<State>& next_state) const = 0;

        // Returns the cost of the path (cumulative sum)
        [[nodiscard]] virtual double path_cost(double c, const std::shared_ptr<State>& state, const std::shared_ptr<Action>& action, const std::shared_ptr<State>& next_state) const {
            return c + step_cost(state, action, next_state);
        };

};

#endif