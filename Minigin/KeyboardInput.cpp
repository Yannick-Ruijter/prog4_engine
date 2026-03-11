#include "KeyboardInput.h"
#include <SDL3/SDL.h>
#include "Binding.h"
#include "Command.h"

using namespace dae;
dae::KeyboardInput::KeyboardInput() = default;
dae::KeyboardInput::~KeyboardInput() = default;
void KeyboardInput::ProcessInput()
{
	m_PreviousState = m_CurrentState;
	m_CurrentState = SDL_GetKeyboardState(nullptr);

	for (auto& binding : m_Bindings)
	{
		if (binding->m_TriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->Execute();
	}
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
	assert(command.get() != nullptr);
	int scancode = ConvertToScancode(keybind);
	m_Bindings.emplace_back(std::make_unique<Binding>(std::move(command), scancode, triggerState));
}

int KeyboardInput::ConvertToScancode(InputKeybinds keybind)
{
	assert(static_cast<int>(keybind) > static_cast<int>(InputKeybinds::KEYBOARD_BEGIN) 
		&& static_cast<int>(keybind) < static_cast<int>(InputKeybinds::KEYBOARD_END) &&
		"Keybind has to be a keyboard keybind");
	//only works for the chars in alphabeth but we don't need more for now
	return 4 + static_cast<int>(keybind) - static_cast<int>(InputKeybinds::A);
}