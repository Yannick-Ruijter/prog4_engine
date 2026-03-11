#pragma once
#include "InputInfo.h"
#include <memory>
#include "Command.h"

namespace dae
{
	class Command;
	class Binding
	{
	public:
		Binding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState);

		std::unique_ptr<Command> m_Command;
		InputKeybinds m_Keybind;
		InputState m_TriggerState;
	};
}