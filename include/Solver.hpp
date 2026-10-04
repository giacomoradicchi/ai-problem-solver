#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Node.hpp"
#include "Problem.hpp"
#include <vector>
#include <memory>
#include <functional>

class Solver {
    private:
        // Checks if the node form a cycle of length k or less 
        [[nodiscard]] bool is_cycle(const std::shared_ptr<Node>& node, int k = 30) const;

        // Auxiliar method for is_cycle(...) one
        [[nodiscard]] bool find_cycle(const std::shared_ptr<Node>& node, const std::shared_ptr<Node>& ancestor, int k) const;

    public:
        // Solves the problem using best first search, with a custom f(n) function. Returns the solution node or a null pointer if not found. 
        [[nodiscard]] std::shared_ptr<Node> best_first_search(const std::shared_ptr<Problem>& problem, const std::function<double(const std::shared_ptr<Node>&)>& f, bool graph_search = true);

        // Solves the problem using breadth first search (BFS). Returns the solution node or a null pointer if not found. 
        [[nodiscard]] std::shared_ptr<Node> breadth_first_search(const std::shared_ptr<Problem>& problem, bool graph_search = true);

        // Solves the problem using depth first search (DFS). Returns the solution node or a null pointer if not found.
        [[nodiscard]] std::shared_ptr<Node> depth_first_search(const std::shared_ptr<Problem>& problem, bool graph_search = false);

        // Solves the problem using iterative deepening depth first search. Returns the solution node or a null pointer if not found.
        [[nodiscard]] std::shared_ptr<Node> iterative_deepening(const std::shared_ptr<Problem>& problem, bool graph_search = false);

        // Solves the problem using uniform-cost search (UCS). Returns the solution node or a null pointer if not found.
        [[nodiscard]] std::shared_ptr<Node> uniform_cost_search(const std::shared_ptr<Problem>& problem, bool graph_search = true);

        // Solves the problem using A* search. Returns the solution node or a null pointer if not found.
        [[nodiscard]] std::shared_ptr<Node> a_star(const std::shared_ptr<Problem>& problem, const std::function<double(const std::shared_ptr<Node>&)>& h, bool graph_search = true);

        // Solves the problem using iterative-deepening A* search (IDA*). Returns the solution node or a null pointer if not found.
        [[nodiscard]] std::shared_ptr<Node> ida_star(const std::shared_ptr<Problem>& problem, const std::function<double(const std::shared_ptr<Node>&)>& h, bool graph_search = true);
        
};

#endif