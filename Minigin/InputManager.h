#pragma once
#include "Singleton.h"
#include <Windows.h>
#pragma comment(lib, "XInput.lib")
#include <XInput.h>
#include <vector>
#include <memory>
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddCommand(std::unique_ptr<Command> command);

		bool WasPressedThisFrame(unsigned int button) const;
		bool IsButtonPressed(unsigned int button) const;
		bool WasReleasedThisFrame(unsigned int button) const;

	private:
		std::vector<std::unique_ptr<Command>> m_Commands;
		XINPUT_STATE m_CurrentState{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};
	};

}
