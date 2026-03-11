#include "ControllerInput.h"

using namespace dae;
dae::ControllerInput::ControllerInput(int controllerIndex)
	:m_ControllerIndex{controllerIndex}
{
	int numberOfGamepadsConnected{};
	auto joystickIDs = SDL_GetGamepads(&numberOfGamepadsConnected);
	m_GamePad = SDL_GetGamepadFromID(joystickIDs[0]);
}
void ControllerInput::ProcessInput()
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}


bool ControllerInput::WasPressedThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool ControllerInput::IsButtonPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool ControllerInput::WasReleasedThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

void ControllerInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
	m_Bindings.emplace_back(std::make_unique<Binding>(std::move(command), (int)keybind, triggerState));
}