#pragma once

namespace dae
{
	class PlayerInput {
	public:
		virtual void ProcessInput() = 0;

		virtual bool WasPressedThisFrame(unsigned int button) const = 0;
		virtual bool IsButtonPressed(unsigned int button) const = 0;
		virtual bool WasReleasedThisFrame(unsigned int button) const = 0;
	}; 
}