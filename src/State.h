#pragma once
class State {
public:
	virtual ~State() = default;
	virtual void on_enter() noexcept = 0;
	virtual void on_execute() noexcept = 0;
	virtual void on_exit() noexcept = 0;
};