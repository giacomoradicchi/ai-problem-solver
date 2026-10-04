#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>

class Action {
    private:
        std::string action_name;
    
    public:
        virtual ~Action() = default;

        explicit Action(std::string action_name) : action_name(std::move(action_name)) {};

        [[nodiscard]] virtual const std::string& to_string() const noexcept {
            return action_name;
        };

        [[nodiscard]] virtual bool operator==(const Action& other) const noexcept {
            return action_name == other.action_name;
        };
        
};

#endif