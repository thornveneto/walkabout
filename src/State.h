#pragma once
template <typename T>
class State {
public:
	virtual ~State() = default;
	virtual void on_enter() noexcept = 0;
	virtual void process_event(const T& event) noexcept = 0;
	virtual void on_exit() noexcept = 0;
};