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

		void AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;
#if _WIN32
		class XInputImpl;
		std::unique_ptr<XInputImpl> m_pXInputImpl;
#else
		class SdlImpl;
		std::unique_ptr<SdlImpl> m_pSdlImpl;
#endif
	private:
		int m_ControllerIndex{ 0 };

		std::vector<std::unique_ptr<Binding>> m_Bindings;
	};
}