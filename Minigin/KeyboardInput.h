#pragma once
#include "PlayerInput.h"

namespace dae
{
	class KeyboardInput : public PlayerInput {
	public:
		void ProcessInput() override;

		bool WasPressedThisFrame(unsigned int button) const override;
		bool IsButtonPressed(unsigned int button) const override;
		bool WasReleasedThisFrame(unsigned int button) const override;

	private:
		const bool* m_CurrentState{ nullptr };
		const bool* m_PreviousState{ nullptr };
	};
}