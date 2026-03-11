#pragma once
#include "PlayerInput.h"
#include "InputInfo.h"
#include <memory>
#include <vector>
#include "Binding.h"

namespace dae
{
	class Command;
	class KeyboardInput : public PlayerInput {
	public:
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

		void AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;

	private:
		int ConvertToScancode(InputKeybinds keybind);

		const bool* m_CurrentState{ nullptr };
		const bool* m_PreviousState{ nullptr };

		std::vector<std::unique_ptr<Binding>> m_Bindings;
	};
}