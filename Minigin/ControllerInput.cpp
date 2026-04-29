#include "ControllerInput.hpp"
#include <array>
#include "Binding.hpp"
#include "Command.hpp"
#include <ranges>
//for testing purposes
#define SDLTEST 1
#if _WIN32
	#include <Windows.h>
	#pragma comment(lib, "XInput.lib")
	#include <XInput.h>
#else
	#include <SDL3/SDL.h>
	#include <SDL3/SDL_gamepad.h>
#endif
using namespace dae;

class ControllerInput::Impl
{
public:
	Impl(int controllerIndex);
	void ProcessInput();

	bool WasPressedThisFrame(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;
	bool WasReleasedThisFrame(unsigned int button) const;

	std::unique_ptr<Binding> AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState = InputState::None);

private:
	int m_ControllerIndex{};
#if _WIN32
	int ConvertToXInput(InputKeybinds keybind);

	XINPUT_STATE m_CurrentState{};
	unsigned int m_ButtonsPressedThisFrame{};
	unsigned int m_ButtonsReleasedThisFrame{};
#else

	int ConvertToSdlKeybind(InputKeybinds keybind);

	SDL_Gamepad* m_GamePad;
	std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_States{};
	std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_PreviousStates{};
#endif
};

ControllerInput::ControllerInput(int controllerIndex)
	:m_pImpl{std::make_unique<Impl>(controllerIndex)}
	,m_Bindings{}
{}

dae::ControllerInput::~ControllerInput() = default;

void ControllerInput::ProcessInput()
{
	m_pImpl->ProcessInput();

	for (auto& binding : m_Bindings)
	{
		if (binding->m_TriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		else if (binding->m_TriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		else if (binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->Execute();

		if (binding->m_EndTriggerState == InputState::None) continue;
		if (binding->m_EndTriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->StopExecution();
		else if (binding->m_EndTriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->StopExecution();
		else if (binding->m_EndTriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->StopExecution();
	}
}

bool ControllerInput::WasPressedThisFrame(unsigned int button) const
{
	return m_pImpl->WasPressedThisFrame(button);
}

bool ControllerInput::IsButtonPressed(unsigned int button) const
{
	return m_pImpl->IsButtonPressed(button);
}

bool ControllerInput::WasReleasedThisFrame(unsigned int button) const
{
	return m_pImpl->WasReleasedThisFrame(button);
}

Binding* ControllerInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState)
{
	m_Bindings.emplace_back(m_pImpl->AddBinding(std::move(command), keybind, triggerState, endTriggerState));
	return m_Bindings.back().get();
}

std::unique_ptr<Binding> ControllerInput::UnBind(Binding* binding)
{
	auto it = std::find_if(begin(m_Bindings), end(m_Bindings), [binding](const auto& ptr) {return ptr.get() == binding; });
	if (it == m_Bindings.end()) return nullptr;
	auto foundBinding = std::move(*it);
	m_Bindings.erase(it);
	return foundBinding;
}

ControllerInput::Impl::Impl(int controllerIndex)
	:m_ControllerIndex{controllerIndex}
{
#if _WIN32
#else
	int numberOfGamepadsConnected{};
	auto joystickIDs = SDL_GetGamepads(&numberOfGamepadsConnected);
	//assert(controllerIndex < numberOfGamepadsConnected && "There is not enough gamepads connected right now");
	m_GamePad = SDL_OpenGamepad(joystickIDs[m_ControllerIndex]);
	SDL_free(joystickIDs);
#endif
}

void ControllerInput::Impl::ProcessInput()
{
#if _WIN32
	XINPUT_STATE previousState;
	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
#else
	if (m_GamePad == nullptr) return;
	m_PreviousStates = std::move(m_States);
	for (int i{ 0 }; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
	{
		m_States[i] = SDL_GetGamepadButton(m_GamePad, static_cast<SDL_GamepadButton>(i));
	}
#endif
}

bool ControllerInput::Impl::WasPressedThisFrame(unsigned int button) const
{
#if _WIN32
	return m_ButtonsPressedThisFrame & button;
#else
	return !m_PreviousStates[button] && m_States[button];
#endif
}

bool ControllerInput::Impl::IsButtonPressed(unsigned int button) const
{
#if _WIN32
	return m_CurrentState.Gamepad.wButtons & button;
#else
	return m_States[button];
#endif
}

bool ControllerInput::Impl::WasReleasedThisFrame(unsigned int button) const
{
#if _WIN32
	return m_ButtonsReleasedThisFrame & button;
#else
	return m_PreviousStates[button] && !m_States[button];
#endif
}

std::unique_ptr<Binding> ControllerInput::Impl::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState)
{
#if _WIN32
	int XinputValue = ConvertToXInput(keybind);
	return std::make_unique<Binding>(std::move(command), XinputValue, triggerState, endTriggerState);
#else
	int keybindValue = ConvertToSdlKeybind(keybind);
	return std::make_unique<Binding>(std::move(command), keybindValue, triggerState, endTriggerState);
#endif
}

#if _WIN32
int ControllerInput::Impl::ConvertToXInput(InputKeybinds keybind)
{
	assert(static_cast<int>(keybind) > static_cast<int>(InputKeybinds::CONTROLLER_BEGIN)
		&& static_cast<int>(keybind) < static_cast<int>(InputKeybinds::CONTROLLER_END) &&
		"Keybind has to be a controller keybind");
	//this works only if the things stay ordered and xinput does not change the layout
	return 1 << (static_cast<int>(keybind) - static_cast<int>(InputKeybinds::DPAD_UP));
}
#else
int ControllerInput::Impl::ConvertToSdlKeybind(InputKeybinds keybind)
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
	default: return -1;
	}
}
#endif