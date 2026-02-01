#pragma once
#include "State.h"
#include <stdexcept>
#include <memory>
template <typename T, typename E>
class StateMachine {
	//?static_assert(std::is_base_of_v<State, T>,"StateMachine<T>: T must derive from State");

	std::unique_ptr<T> current_state;//mandatory parameter, class invariant
public:
	StateMachine(std::unique_ptr<T> initial_state) : current_state { std::move(initial_state)} {
		if (!current_state) {
			throw std::invalid_argument("initial current_state cannot be nullptr");
		}
		current_state->on_enter();
	}
	
	StateMachine(const StateMachine&) = delete;
	StateMachine& operator=(const StateMachine&) = delete;

	StateMachine(StateMachine&&) = delete;
	StateMachine& operator=(StateMachine&&) = delete;

	void process_event(const E& event) {
		current_state->process_event(event);
	}

	T* get_current_state() const {
		return current_state.get();
	}

	void switch_state(std::unique_ptr<T> new_state) {
		if (!new_state) {
			throw std::invalid_argument("cannot switch to nullptr new state");
		}

		current_state->on_exit();

		current_state = std::move(new_state);

		current_state->on_enter();
	}
};