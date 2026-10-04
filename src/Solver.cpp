#include "Solver.hpp"

#include <memory>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#define FAILURE nullptr

// structs for reached list (hash map)
struct StateHash {
    std::size_t operator()(const std::shared_ptr<State>& state) const noexcept {
        if (state) return state->hash();   

        return 0u;   // 0 if state is nullptr (u = unsigned)
    }
};

struct StateEqual {
    bool operator()(const std::shared_ptr<State>& lhs, const std::shared_ptr<State>& rhs) const noexcept { // lhs= left hand side, rhs = right hand side (defines the order of the comparison)
        if (lhs == rhs) return true; // the same pointer -> true

        if (!lhs || !rhs) return false; // one is the null pointer and the other not -> false (this condition works as a XOR, since the case where both are nullptr is before)

        return *lhs == *rhs; // both are != nullptr, so uses the == operator to check if they are the same
    }
};

std::shared_ptr<Node> Solver::breadth_first_search(const std::shared_ptr<Problem>& problem, bool graph_search) {
    
    // check if root is the goal
    auto root = std::make_shared<Node>(problem->initial());
    if (problem->is_goal_state(root->get_state())) {
        return root;
    }

    // frontier: using a FIFO queue
    std::queue<std::shared_ptr<Node>> frontier;

    // adding initial node to the frontier
    frontier.push(root);

    // reached list (only if graph_search == true)
    // using a list instead of hash map for DFS
    /* std::unordered_map<
        std::shared_ptr<State>,     // key
        std::shared_ptr<Node>,      // value
        StateHash,                  // hash handler (manages also nullptr)
        StateEqual                  // equal operator handler (manages also nullptr)
    > reached; 
    if (graph_search) {
        reached[root->get_state()] = root;
    } */
    std::unordered_set<
        std::shared_ptr<State>,     // set type
        StateHash,                  // hash handler (manages also nullptr)
        StateEqual                  // equal operator handler (manages also nullptr)
    > reached; 
    if (graph_search) {
        reached.insert(root->get_state());
    }

    while (!frontier.empty()) {
        auto node = frontier.front();  // read front
        frontier.pop();                // remove front

        // expand node
        for (const auto& child : node->expand(problem)) {
            auto child_state = child->get_state();

            // goal check when child is generated (reduced complexity)
            if (problem->is_goal_state(child_state)) {
                return child;
            }

            // tree search    or (graph search and) the result of reached.insert is true (so there was no child_state before, since reached is a set)
            if (!graph_search || reached.insert(child_state).second) {
                frontier.push(child);
            }

        }
    }

    return FAILURE;
}