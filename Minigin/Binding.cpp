#include "Binding.h"
#include "Command.h"

using namespace dae;

Binding::Binding(std::unique_ptr<Command> command, int keybind, InputState triggerState)
	:m_Command{std::move(command)}
	, m_Keybind{keybind}
	, m_TriggerState{triggerState}
{

}

dae::Binding::~Binding() = default;
