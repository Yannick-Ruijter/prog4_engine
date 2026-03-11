#include "KeyboardInput.h"
#include <SDL3/SDL.h>
#include "Binding.h"

using namespace dae;
void KeyboardInput::ProcessInput()
{
	m_PreviousState = m_CurrentState;
	m_CurrentState = SDL_GetKeyboardState(nullptr);
}

bool KeyboardInput::WasPressedThisFrame(unsigned int button) const
{
	if (m_PreviousState == nullptr)
		return m_CurrentState[button];
	bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];
	
	return buttonChange && m_CurrentState[button];
}

bool KeyboardInput::IsButtonPressed(unsigned int button) const
{
	return m_CurrentState[button];
}

bool KeyboardInput::WasReleasedThisFrame(unsigned int button) const
{
	if (m_PreviousState == nullptr)
		return !m_CurrentState[button];
	bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];

	return buttonChange && !m_CurrentState[button];
}

void KeyboardInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
	m_Bindings.emplace_back(std::make_unique<Binding>(std::move(command), keybind, triggerState));
}