#include "Binding.hpp"
#include "Command.hpp"

using namespace dae;

Binding::Binding(std::unique_ptr<Command> command, int keybind, InputState triggerState, InputState endTriggerState)
	:m_Command{std::move(command)}
	, m_Keybind{keybind}
	, m_TriggerState{triggerState}
	, m_EndTriggerState{endTriggerState}
{

}

dae::Binding::~Binding() = default;
