#pragma once
#include "Singleton.hpp"
#include <vector>
#include <memory>
#include "Command.hpp"
#include "ControllerInput.hpp"
#include "KeyboardInput.hpp"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		ControllerInput* GetControllerInput(int controllerIndex);
		KeyboardInput* GetKeyboardInput();
	private:
		ControllerInput m_ControllerInput[4]{0, 1, 2, 3};
		KeyboardInput m_KeyboardInput{};
	};

}
