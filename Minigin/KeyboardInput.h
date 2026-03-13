#pragma once
#include "PlayerInput.h"
#include "InputInfo.h"
#include <memory>
#include <vector>

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

		Binding* AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;
		std::unique_ptr<Binding> UnBind(Binding* binding) override;

	private:
		int ConvertToScancode(InputKeybinds keybind);

		const bool* m_CurrentState{ nullptr };
		const bool* m_PreviousState{ nullptr };

		std::vector<std::unique_ptr<Binding>> m_Bindings;
	};
}