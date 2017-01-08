#pragma once
#include <iostream>
#include <vector>
#include <functional>

namespace fsm {


/**
 * @brief      Class for state machine.
 *
 * @tparam     Enum  A enum class of states
 */
template <typename Enum>
class state_machine
{
    public: struct state
    {
        Enum current_state;
        Enum next_state;

        // This function is called when the state is entered
        std::function<void()> trigger   = [] () {};

        // conditional guard requirement which must be satisfied to visit state
        std::function<bool()> condition = [] () { return true; };
    };

    private: std::vector<state> transition_matrix_;
    private: Enum current_state_;

    public: state_machine
    (   Enum initial_state,
        std::vector<state> transition_matrix
    )
    :   transition_matrix_(transition_matrix)
    ,   current_state_(initial_state)
    {};

    /**
     * Attempts to move state machine onto next state if applicable
     */
    public: void next()
    {
        std::cout << "next()\n";

        std::vector<state> valid_next_states;
        std::copy_if(
            transition_matrix_.begin(),
            transition_matrix_.end(),
            std::back_inserter(valid_next_states),
            std::bind(
                &state_machine::condition_satisfied,
                this,
                std::placeholders::_1));

        if (valid_next_states.size() == 1)
        {
            transition(valid_next_states[0]);
        }

        else if (valid_next_states.size() > 1)
        {
            // uh oh, non-determinism found.
            std::cout << "Non-deterministic state machine.\n";
        }
    }

    /**
     * Checks to see if state can be entered
     * @param  state a given state in the transition matrix
     * @return       true if state can be entered
     */
    private: bool condition_satisfied(const state state)
    {
        return (state.current_state == current_state_ &&
                state.condition() == true);
    }

    private: void transition(const state state)
    {
        std::cout << "transition()\n";
        state.trigger();
        current_state_ = state.next_state;
        std::cout
          << static_cast<std::uint32_t>(state.current_state)
          << " -> "
          << static_cast<std::uint32_t>(state.next_state)
          << "\n";
    }
};

}
