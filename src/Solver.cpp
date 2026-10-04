#include "Solver.hpp"

#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#define FAILURE nullptr

// funntor for priority queue
struct NodeComparator {
    std::function<double(const std::shared_ptr<Node>&)> f;

    bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
        return f(lhs) > f(rhs); // switching "<" with ">" because priority_queue is a max heap (and we want a min heap).
    }
};

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

std::shared_ptr<Node> Solver::best_first_search(const std::shared_ptr<Problem>& problem, const std::function<double(const std::shared_ptr<Node>&)>& f, bool graph_search) {

    // init comparator (for min_heap behaviour)
    NodeComparator comparator{f};

    // init frontier
    std::priority_queue<
        std::shared_ptr<Node>,
        std::vector<std::shared_ptr<Node>>,
        NodeComparator
    > frontier(comparator);

    auto root = std::make_shared<Node>(problem->initial());
    frontier.push(root);

    // reached list (if graph search enabled)
    std::unordered_map<
        std::shared_ptr<State>,     // key
        std::shared_ptr<Node>,      // value
        StateHash,                  // hash handler (manages also nullptr)
        StateEqual                  // equal operator handler (manages also nullptr)
    > reached;
    if (graph_search) {
        reached[root->get_state()] = root;
    }

    while (!frontier.empty()) {
        std::shared_ptr<Node> node = frontier.top();
        frontier.pop();

        // goal check when node is popped
        if (problem->is_goal_state(node->get_state())) {
            return node;
        }

        // node expansion
        for (const auto& child : node->expand(problem)) {
            auto child_state = child->get_state();

            // tree search
            if (!graph_search) {
                frontier.push(child);
                continue;
            }

            // graph search
            auto existing_entry = reached.find(child_state); // returns the position of the existing node for the child_state if it exists, the end of the reached map data structure otherwise.
            if (existing_entry == reached.end() // child_state is not in reached (existing_node associated with child_state not found) 
            || f(child) < f(existing_entry->second)) { // existing_entry has first:state, second:node. in order to get the associated node, we use existing_entry->second.
                reached[child_state] = child;
                frontier.push(child);
            }
        }

    }

    return FAILURE;

}


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

std::shared_ptr<Node> Solver::depth_first_search(const std::shared_ptr<Problem>& problem, bool graph_search) {

    // init frontier
    std::stack<std::shared_ptr<Node>> frontier;
    auto root = std::make_shared<Node>(problem->initial());
    frontier.push(root);

    // reached list (if graph search enabled)
    std::unordered_map<
        std::shared_ptr<State>,
        std::shared_ptr<Node>,
        StateHash,
        StateEqual
    > reached;
    if (graph_search) {
        reached[root->get_state()] = root;
    }
    
    while (!frontier.empty()) {
        std::shared_ptr<Node> node = frontier.top();
        frontier.pop();

        // goal check when node is popped
        if (problem->is_goal_state(node->get_state())) {
            return node;
        }

        if (is_cycle(node)) continue; // avoid cycles

        // node expansion
        for (const auto& child : node->expand(problem)) {
            auto child_state = child->get_state();

            // tree search
            if (!graph_search) {
                frontier.push(child);
                continue;
            }

            // graph search
            auto existing_entry = reached.find(child_state); // returns the position of the existing node for the child_state if it exists, the end of the reached map data structure otherwise.
            if (existing_entry == reached.end() // child_state is not in reached (existing_node associated with child_state not found) 
            || child->get_path_cost() < existing_entry->second->get_path_cost()) { // existing_entry has first:state, second:node. in order to get the associated node, we use existing_entry->second.
                reached[child_state] = child;
                frontier.push(child);
            }
        }

    }

    return FAILURE;
}

bool Solver::is_cycle(const std::shared_ptr<Node>& node, int k) const {
    return find_cycle(node, node->get_parent(), k);
}

bool Solver::find_cycle(const std::shared_ptr<Node>& node, const std::shared_ptr<Node>& ancestor, int k) const {
    return ancestor && k > 0 && (*ancestor->get_state() == *node->get_state() || find_cycle(node, ancestor->get_parent(), k - 1));
}

std::shared_ptr<Node> Solver::uniform_cost_search(const std::shared_ptr<Problem>& problem, bool graph_search) {
    auto g = [](const std::shared_ptr<Node>& node) -> double {
        return node->get_path_cost();
    };

    return best_first_search(problem, g, graph_search);
}

std::shared_ptr<Node> Solver::a_star(const std::shared_ptr<Problem>& problem, const std::function<double(const std::shared_ptr<Node>&)>& h, bool graph_search) {
    auto f = [&h](const std::shared_ptr<Node>& node) -> double {
        return node->get_path_cost() + h(node);
    };

    return best_first_search(problem, f, graph_search);
}