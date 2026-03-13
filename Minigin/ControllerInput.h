#pragma once
#include "PlayerInput.h"
#include "InputInfo.h"
#include <memory>
#include <vector>

namespace dae
{
	class Command;
	class Binding;
	class ControllerInput : public PlayerInput {
	public:
		ControllerInput(int controllerIndex);
		~ControllerInput();
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

		Binding* AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;
		std::unique_ptr<Binding> UnBind(Binding* binding) override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
		std::vector<std::unique_ptr<Binding>> m_Bindings;
	};
}