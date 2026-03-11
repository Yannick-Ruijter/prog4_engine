#pragma once
#include "InputInfo.h"
#include <memory>
namespace dae
{
	class Command;
	class PlayerInput {
	public:
		virtual void ProcessInput() = 0;

		virtual bool WasPressedThisFrame(unsigned int button) const = 0;
		virtual bool IsButtonPressed(unsigned int button) const = 0;
		virtual bool WasReleasedThisFrame(unsigned int button) const = 0;

		virtual void AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) = 0;
	}; 
}