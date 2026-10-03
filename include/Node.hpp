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
        double path_cost{0.0};
        unsigned int depth{0};

    public:
        Node(std::shared_ptr<State> state, std::shared_ptr<Node> parent, std::shared_ptr<Action> action, double path_cost);

        std::vector<std::shared_ptr<Node>> expand(std::unique_ptr<Problem> problem);
        std::shared_ptr<Node> child_node(std::unique_ptr<Problem> problem, std::shared_ptr<Action> action);

};

#endif