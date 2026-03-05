#define WIN32_LEAN_AND_MEAN
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include "Command.h"

bool dae::InputManager::ProcessInput()
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {

		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

		}
		// etc...
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	for (auto const& command : m_Commands)
	{
		command->Execute();
	}
	return true;
}

void dae::InputManager::AddCommand(std::unique_ptr<Command> command)
{
	m_Commands.push_back(std::move(command));
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
