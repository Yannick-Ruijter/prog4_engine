#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Command.h"
#include "ControllerInput.h"
#include "KeyboardInput.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddCommand(std::unique_ptr<Command> command);
		ControllerInput* GetControllerInput(int controllerIndex);
		KeyboardInput* GetKeyboardInput();
	private:
		std::vector<std::unique_ptr<Command>> m_Commands{};
		ControllerInput m_ControllerInput[4]{0, 1, 2, 3};
		KeyboardInput m_KeyboardInput{};
	};

}
