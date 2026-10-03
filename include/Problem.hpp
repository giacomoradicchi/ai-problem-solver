#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <memory>
#include <vector>
#include "Node.hpp"
#include "State.hpp"
#include "Action.hpp"


class Problem {
    public:
        virtual ~Problem();
        
        virtual std::shared_ptr<Node> initial() const = 0;
        virtual bool is_goal_state(std::shared_ptr<State> state) const = 0;
        virtual std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) const = 0;
        virtual std::shared_ptr<State> result(std::shared_ptr<State> state, std::shared_ptr<Action> action) const = 0;
        virtual double step_cost(std::shared_ptr<State> state, std::shared_ptr<Action> action, std::shared_ptr<State> next_state) const = 0;
        virtual double path_cost(double c, std::shared_ptr<State> state, std::shared_ptr<Action> action, std::shared_ptr<State> next_state) const = 0;

};

#endif