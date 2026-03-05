#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <Windows.h>
bool dae::InputManager::ProcessInput()
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	return true;
}

bool dae::InputManager::WasPressedThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool dae::InputManager::IsButtonPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool dae::InputManager::WasReleasedThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}
