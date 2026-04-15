#pragma once
#include "InputInfo.h"
#include <memory>

namespace dae
{
	class Command;
	class Binding
	{
	public:
		Binding(std::unique_ptr<Command> command, int keybind, InputState triggerState, InputState endTriggerState = InputState::None);
		~Binding();
		std::unique_ptr<Command> m_Command;
		int m_Keybind;
		InputState m_TriggerState;
		InputState m_EndTriggerState;
	};
}