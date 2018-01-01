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
    public: template<class F, class...Args>
    static auto guard(F&& f, Args&&... args) -> std::function<bool()>
    {
        return std::bind(f, args...);
    }

    public: template<class F, class...Args>
    static auto event(F&& f, Args&&... args) -> std::function<void()>
    {
        return std::bind(f, args...);
    }

    public: struct state
    {
        using event_type = std::function<void()>;
        using guard_type = std::function<bool()>;
        Enum current_state;
        Enum next_state;

        // This function is called when the state is entered
        event_type event;

        // conditional guard requirement which must be satisfied to visit state
        guard_type guard;

        state(Enum current_state, Enum next_state)
        :   current_state(current_state)
        ,   next_state(next_state) {}

        state(Enum current_state, Enum next_state, event_type event)
        :   current_state(current_state)
        ,   next_state(next_state)
        ,   event(event) {}

        state(Enum current_state, Enum next_state, guard_type guard)
        :   current_state(current_state)
        ,   next_state(next_state)
        ,   guard(guard) {}

        state(Enum current_state, Enum next_state, 
            guard_type guard, 
            event_type event)
        :   current_state(current_state)
        ,   next_state(next_state)
        ,   guard(guard) 
        ,   event(event) {}

        state(Enum current_state, Enum next_state, 
            event_type event,
            guard_type guard)
        :   current_state(current_state)
        ,   next_state(next_state)
        ,   event(event)
        ,   guard(guard) {}
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
                &state_machine::guard_satisfied,
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
    private: bool guard_satisfied(const state state)
    {
        return (state.current_state == current_state_ &&
                state.guard() == true);
    }

    private: void transition(const state state)
    {
        std::cout << "transition()\n";
        state.event();
        current_state_ = state.next_state;
        std::cout
          << static_cast<std::uint32_t>(state.current_state)
          << " -> "
          << static_cast<std::uint32_t>(state.next_state)
          << "\n";
    }
};

}
