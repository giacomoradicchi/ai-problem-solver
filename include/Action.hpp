#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
class Action {
    private:
        std::string action_name;
    
    public:
        Action(std::string action_name);

        std::string to_string();
};

#endif