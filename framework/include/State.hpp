#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <string>

class State {
    public:
        virtual ~State() = default;

        [[nodiscard]] virtual bool operator==(const State& other) const = 0;

        // returns the hash code of the State (helpful for the reached list, which is a hash map) 
        [[nodiscard]] virtual std::size_t hash() const = 0;

        [[nodiscard]] virtual std::string to_string() const = 0;
        
};

#endif