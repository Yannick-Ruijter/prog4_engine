#pragma once
#include "InputInfo.hpp"
#include <memory>
namespace dae
{
	class Command;
	class Binding;
	class PlayerInput {
	public:
		virtual void ProcessInput() = 0;

		virtual bool WasPressedThisFrame(unsigned int button) const = 0;
		virtual bool IsButtonPressed(unsigned int button) const = 0;
		virtual bool WasReleasedThisFrame(unsigned int button) const = 0;

		virtual Binding* AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState = InputState::None) = 0;
		virtual std::unique_ptr<Binding> UnBind(Binding* binding) = 0;
	}; 
}