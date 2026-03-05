#pragma once
#include "Singleton.h"
#include <XInput.h>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		bool WasPressedThisFrame(unsigned int button) const;
		bool IsButtonPressed(unsigned int button) const;
		bool WasReleasedThisFrame(unsigned int button) const;
	private:
		XINPUT_STATE m_CurrentState{};
		unsigned int m_ButtonsPressedThisFrame{};
		unsigned int m_ButtonsReleasedThisFrame{};
	};

}
