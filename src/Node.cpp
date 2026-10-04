#include "Node.hpp"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Node>> Node::expand(const std::shared_ptr<Problem>& problem) {
    std::vector<std::shared_ptr<Node>> child_nodes;

    std::vector<std::shared_ptr<Action>> applicable_actions = problem->actions(state);

    child_nodes.reserve(applicable_actions.size()); // allocate the right amount of space (avoiding dinamic reallocations)

    for (const auto& action : applicable_actions) { // const & = constant pointer (no useless copy of the type action, only reading). auto: the compiler auto-detects the type of action (std::shared_ptr<Action>)
        child_nodes.push_back(child_node(problem, action));
    }

    return child_nodes;
}

std::shared_ptr<Node> Node::child_node(const std::shared_ptr<Problem>& problem, const std::shared_ptr<Action>& action) {
    std::shared_ptr<State> next_state = problem->result(state, action);

    return std::make_shared<Node>(
        std::move(next_state), // child state (move(next_state) to reduce pointer operation)
        shared_from_this(), // this node (parent)
        action, 
        problem->path_cost(path_cost, state, action, next_state)
    );
}