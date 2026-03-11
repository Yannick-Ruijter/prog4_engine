#pragma once
//#if _WIN32
#include <Windows.h>
//#else
#include <SDL3/SDL_gamepad.h>
//#endif
#pragma comment(lib, "XInput.lib")
#include <XInput.h>
#include "PlayerInput.h"
#include "InputInfo.h"
#include <memory>
#include <vector>
#include "Binding.h"

namespace dae
{
	class Command;
	class ControllerInput : public PlayerInput {
	public:
		ControllerInput(int controllerIndex);
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

		void AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;

	private:
		int m_ControllerIndex{ 0 };
//#if _WIN32
		XINPUT_STATE m_CurrentState{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};
//#else
		SDL_Gamepad* m_GamePad{};
//#endif

		std::vector<std::unique_ptr<Binding>> m_Bindings{};
	};
}