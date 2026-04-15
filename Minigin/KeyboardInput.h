#pragma once
#include "PlayerInput.h"
#include "InputInfo.h"
#include <memory>
#include <vector>
#include <SDL3/SDL.h>
#include <array>

namespace dae
{
	class Command;
	class Binding;
	class KeyboardInput : public PlayerInput {
	public:
		KeyboardInput();
		~KeyboardInput();
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

		Binding* AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState = InputState::None) override;
		std::unique_ptr<Binding> UnBind(Binding* binding) override;

	private:
		int ConvertToScancode(InputKeybinds keybind);

		const bool* m_CurrentState{ nullptr };
		std::unique_ptr<bool[]> m_PreviousState{};

		std::vector<std::unique_ptr<Binding>> m_Bindings;
	};
}