#ifndef NODE_HPP
#define NODE_HPP

#include "State.hpp"
#include "Action.hpp"
#include "Problem.hpp"
#include <memory>
#include <vector>

class Node : public std::enable_shared_from_this<Node>{

    private:
        std::shared_ptr<State> state;
        std::shared_ptr<Node> parent;
        std::shared_ptr<Action> action;
        double path_cost{0.0};  // default path_cost = 0.0
        unsigned int depth{0};  // default depth = 0

    public:

        // Root constructor. "explicit" guarauntees that C++ won't convert the argument passed as a parameter if it's different from the type Node.

        // Create a search tree Node, specifically the root.
        explicit Node(std::shared_ptr<State> state, double path_cost = 0.0, unsigned int depth = 0) 
            : state(std::move(state)), parent(nullptr), action(nullptr), path_cost(path_cost), depth(depth)
        {};

        // Create a search tree Node, derived from a parent by an action.
        Node(std::shared_ptr<State> state, std::shared_ptr<Node> parent, std::shared_ptr<Action> action, double path_cost)
            : state(std::move(state)), parent(std::move(parent)), action(std::move(action)), path_cost(path_cost), depth(this->parent ? this->parent->get_depth() + 1: 0) // if the parent exists, the depth is the depth of its parent + 1, otherwise it's just 0
        {};

        // Destroys the ancestor chain iteratively (the default recursive destruction overflows the stack on very deep paths, e.g. DFS).
        ~Node();

        // List the nodes reachable in one step from this node.
        std::vector<std::shared_ptr<Node>> expand(const std::shared_ptr<Problem>& problem);

        /* // since it's reading only, we can use the constant pointer of the problem so the performance are enhanced */

        // [Figure 3.10]
        std::shared_ptr<Node> child_node(const std::shared_ptr<Problem>& problem, const std::shared_ptr<Action>& action);

        /* 
        getters. (const == only reading).
        [[nodiscard]] means that the compiler will tell whether this function is called without saving/reading the output. 
        noexcept tells the compiler this method won't ever throw exception (so the machine code should be simpler and the program faster)
         */
        
        
        [[nodiscard]] std::shared_ptr<State> get_state() const noexcept { 
            return state; 
        } 

        [[nodiscard]] std::shared_ptr<Node> get_parent() const noexcept {
            return parent;
        }

        [[nodiscard]] std::shared_ptr<Action> get_action() const noexcept {
            return action;
        }

        [[nodiscard]] double get_path_cost() const noexcept {
            return path_cost;
        }

        [[nodiscard]] unsigned int get_depth() const noexcept {
            return depth;
        }
};

#endif