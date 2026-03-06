#pragma once
#include <Windows.h>
#pragma comment(lib, "XInput.lib")
#include <XInput.h>
#include "PlayerInput.h"

namespace dae
{
	class ControllerInput : public PlayerInput {
	public:
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

	private:
		XINPUT_STATE m_CurrentState{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};
	};
}