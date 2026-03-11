#include "ControllerInput.h"
#include <array>
#include "Binding.h"
#include "Command.h"
#if _WIN32
	#include <Windows.h>
	#pragma comment(lib, "XInput.lib")
	#include <XInput.h>
#else
	#include <SDL3/SDL.h>
	#include <SDL3/SDL_gamepad.h>
#endif
using namespace dae;

#if _WIN32
class ControllerInput::XInputImpl
{
public:
	void ProcessInput(int controllerIndex);

	bool WasPressedThisFrame(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;
	bool WasReleasedThisFrame(unsigned int button) const;

	std::unique_ptr<Binding> AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState);

private:
	int ConvertToXInput(InputKeybinds keybind);

	XINPUT_STATE m_CurrentState{};
	unsigned int m_ButtonsPressedThisFrame{};
	unsigned int m_ButtonsReleasedThisFrame{};
};
#else
class ControllerInput::SdlImpl
{
public:
	ControllerInput::SdlImpl(int controllerIndex);
	void ProcessInput();

	bool WasPressedThisFrame(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;
	bool WasReleasedThisFrame(unsigned int button) const;

	std::unique_ptr<Binding> AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState);

private:
	int ConvertToSdlKeybind(InputKeybinds keybind);

	SDL_Gamepad* m_GamePad;
	std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_States{};
	std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_PreviousStates{};
};
#endif

ControllerInput::ControllerInput(int controllerIndex)
	:m_ControllerIndex{controllerIndex}
	, m_Bindings{}
#if _WIN32
	, m_pXInputImpl{std::make_unique<XInputImpl>()}
#else
	, m_pSdlImpl{ std::make_unique<SdlImpl>(controllerIndex)}
#endif
{}

dae::ControllerInput::~ControllerInput() = default;

void ControllerInput::ProcessInput()
{
#if _WIN32
	m_pXInputImpl->ProcessInput(m_ControllerIndex);
#else
	m_pSdlImpl->ProcessInput();
#endif

	for (auto& binding : m_Bindings)
	{
		if (binding->m_TriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->Execute();
	}
}

bool ControllerInput::WasPressedThisFrame(unsigned int button) const
{
#if _WIN32
	return m_pXInputImpl->WasPressedThisFrame(button);
#else
	return m_pSdlImpl->WasPressedThisFrame(button);
#endif
}

bool ControllerInput::IsButtonPressed(unsigned int button) const
{
#if _WIN32
	return m_pXInputImpl->IsButtonPressed(button);
#else
	return m_pSdlImpl->IsButtonPressed(button);
#endif
}

bool ControllerInput::WasReleasedThisFrame(unsigned int button) const
{
#if _WIN32
	return m_pXInputImpl->WasReleasedThisFrame(button);
#else
	return m_pSdlImpl->WasReleasedThisFrame(button);
#endif
}

void ControllerInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
#if _WIN32
	m_Bindings.emplace_back(m_pXInputImpl->AddBinding(std::move(command), keybind, triggerState));
#else
	m_Bindings.emplace_back(m_pSdlImpl->AddBinding(std::move(command), keybind, triggerState));
#endif
}
#if _WIN32
void ControllerInput::XInputImpl::ProcessInput(int controllerIndex)
{
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(controllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool ControllerInput::XInputImpl::WasPressedThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool ControllerInput::XInputImpl::IsButtonPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

bool ControllerInput::XInputImpl::WasReleasedThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

std::unique_ptr<Binding> ControllerInput::XInputImpl::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
	int XinputValue = ConvertToXInput(keybind);
	return std::make_unique<Binding>(std::move(command), XinputValue, triggerState);
}

int ControllerInput::XInputImpl::ConvertToXInput(InputKeybinds keybind)
{
	assert(static_cast<int>(keybind) > static_cast<int>(InputKeybinds::CONTROLLER_BEGIN)
		&& static_cast<int>(keybind) < static_cast<int>(InputKeybinds::CONTROLLER_END) &&
		"Keybind has to be a controller keybind");
	//this works only if the things stay ordered and xinput does not change the layout
	return 1 << (static_cast<int>(keybind) - static_cast<int>(InputKeybinds::DPAD_UP));
}
#else

ControllerInput::SdlImpl::SdlImpl(int controllerIndex)
{
	int numberOfGamepadsConnected{};
	auto joystickIDs = SDL_GetGamepads(&numberOfGamepadsConnected);
	//assert(controllerIndex < numberOfGamepadsConnected && "There is not enough gamepads connected right now");
	m_GamePad = SDL_OpenGamepad(joystickIDs[controllerIndex]);
	SDL_free(joystickIDs);
}

void ControllerInput::SdlImpl::ProcessInput()
{
	if (m_GamePad == nullptr) return;
	m_PreviousStates = std::move(m_States);
	for (int i{ 0 }; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
	{
		m_States[i] = SDL_GetGamepadButton(m_GamePad, static_cast<SDL_GamepadButton>(i));
	}
}

bool ControllerInput::SdlImpl::WasPressedThisFrame(unsigned int button) const
{
	return !m_PreviousStates[button] && m_States[button];
}

bool ControllerInput::SdlImpl::IsButtonPressed(unsigned int button) const
{
	return m_States[button];
}

bool ControllerInput::SdlImpl::WasReleasedThisFrame(unsigned int button) const
{
	return m_PreviousStates[button] && !m_States[button];
}

std::unique_ptr<Binding> ControllerInput::SdlImpl::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
	int keybindValue = ConvertToSdlKeybind(keybind);
	return std::make_unique<Binding>(std::move(command), keybindValue, triggerState);
}

int ControllerInput::SdlImpl::ConvertToSdlKeybind(InputKeybinds keybind)
{
	switch (keybind)
	{
	case InputKeybinds::DPAD_UP: return SDL_GAMEPAD_BUTTON_DPAD_UP;
	case InputKeybinds::DPAD_DOWN: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
	case InputKeybinds::DPAD_LEFT: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
	case InputKeybinds::DPAD_RIGHT: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
	case InputKeybinds::BUTTON_EAST: return SDL_GAMEPAD_BUTTON_EAST;
	case InputKeybinds::BUTTON_SOUTH: return SDL_GAMEPAD_BUTTON_SOUTH;
	case InputKeybinds::BUTTON_WEST: return SDL_GAMEPAD_BUTTON_WEST;
	case InputKeybinds::START: return SDL_GAMEPAD_BUTTON_START;
	case InputKeybinds::BACK: return SDL_GAMEPAD_BUTTON_BACK;
	case InputKeybinds::LEFT_THUMB: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
	case InputKeybinds::RIGHT_THUMB: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
	case InputKeybinds::LEFT_SHOULDER: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
	case InputKeybinds::RIGHT_SHOULDER: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
	}
	return -1;
}
#endif
