#include "Binding.hpp"
#include "Command.hpp"
#include "ControllerInput.hpp"
#include <array>
#include <map>
#include <ranges>
// for testing purposes
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

    bool WasPressedThisFrame(InputAction button) const;
    bool IsButtonPressed(InputAction button) const;
    bool WasReleasedThisFrame(InputAction button) const;

    bool WasPressedThisFrame(unsigned int button) const;
    bool IsButtonPressed(unsigned int button) const;
    bool WasReleasedThisFrame(unsigned int button) const;

    void BindInputAction(InputAction action, InputKeybinds keybind);

    std::unique_ptr<Binding>
    AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState);

  private:
    std::map<InputAction, unsigned int> m_ActionsMapped{};
    int m_ControllerIndex{};
#if _WIN32
    int ConvertToXInput(InputKeybinds keybind);

    XINPUT_STATE m_CurrentState{};
    unsigned int m_ButtonsPressedThisFrame{};
    unsigned int m_ButtonsReleasedThisFrame{};
#else

    int ConvertToSdlKeybind(InputKeybinds keybind);

    SDL_Gamepad *m_GamePad;
    std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_States{};
    std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_PreviousStates{};
#endif
};

ControllerInput::ControllerInput(int controllerIndex) : m_pImpl{std::make_unique<Impl>(controllerIndex)}, m_Bindings{}
{
}

dae::ControllerInput::~ControllerInput() = default;

void ControllerInput::ProcessInput()
{
    UpdateBindings();
    m_pImpl->ProcessInput();

    for (auto &binding : m_Bindings)
    {
        if (binding->m_TriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind))
            binding->m_Command->Execute();
        else if (binding->m_TriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind))
            binding->m_Command->Execute();
        else if (binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind))
            binding->m_Command->Execute();
    }
}

bool dae::ControllerInput::WasPressedThisFrame(InputAction button) const
{
    return m_pImpl->WasPressedThisFrame(button);
}

bool dae::ControllerInput::IsButtonPressed(InputAction button) const
{
    return m_pImpl->IsButtonPressed(button);
}

bool dae::ControllerInput::WasReleasedThisFrame(InputAction button) const
{
    return m_pImpl->WasReleasedThisFrame(button);
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

PlayerInput &dae::ControllerInput::BindInputAction(InputAction action, InputKeybinds keybind)
{
    m_pImpl->BindInputAction(action, keybind);
    return *this;
}

Binding *ControllerInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{

    // i save it here into this array to already be able to return the binding to the caller
    auto binding = m_BindingsToAdd.emplace_back(m_pImpl->AddBinding(std::move(command), keybind, triggerState)).get();

    // adding it to the queue of commands to execute when possible
    // we do this because this might be called while looping over the bindings
    m_BindingCommandQueue.emplace(
        [&, binding]()
        {
            auto it = std::find_if(
                m_BindingsToAdd.begin(), m_BindingsToAdd.end(),
                [binding](auto &other) { return other.get() == binding; });

            if (it != m_BindingsToAdd.end())
            {
                m_Bindings.emplace_back(std::move(*it));
                std::erase(m_BindingsToAdd, *it);
            }
        });
    return binding;
}

void ControllerInput::UnBind(Binding *binding)
{
    // adding it to the queue of commands to execute when possible
    // we do this because this might be called while looping over the bindings
    // clang-format off
    m_BindingCommandQueue.emplace(
        [&, binding]() 
        {
            std::erase_if(m_Bindings, [&, binding](auto &other) { return other.get() == binding; }); 
        });
    // clang format on
}

void dae::ControllerInput::UpdateBindings()
{
    while (!m_BindingCommandQueue.empty())
    {
        //it's a function
        m_BindingCommandQueue.front()();
        m_BindingCommandQueue.pop();
    }
}

ControllerInput::Impl::Impl(int controllerIndex) : m_ControllerIndex{controllerIndex}
{
#if _WIN32
#else
    int numberOfGamepadsConnected{};
    auto joystickIDs = SDL_GetGamepads(&numberOfGamepadsConnected);
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
    if (m_GamePad == nullptr)
        return;
    m_PreviousStates = std::move(m_States);
    for (int i{0}; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
    {
        m_States[i] = SDL_GetGamepadButton(m_GamePad, static_cast<SDL_GamepadButton>(i));
    }
#endif
}

bool ControllerInput::Impl::WasPressedThisFrame(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return WasPressedThisFrame(m_ActionsMapped.at(button));
}

bool ControllerInput::Impl::IsButtonPressed(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return IsButtonPressed(m_ActionsMapped.at(button));
}

bool ControllerInput::Impl::WasReleasedThisFrame(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return WasReleasedThisFrame(m_ActionsMapped.at(button));
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

void ControllerInput::Impl::BindInputAction(InputAction action, InputKeybinds keybind)
{
#if _WIN32
    m_ActionsMapped[action] = ConvertToXInput(keybind);
#else
    m_ActionsMapped[action] = ConvertToSdlKeybind(keybind);
#endif
}

std::unique_ptr<Binding>
ControllerInput::Impl::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
#if _WIN32
    int XinputValue = ConvertToXInput(keybind);
    return std::make_unique<Binding>(std::move(command), XinputValue, triggerState);
#else
    int keybindValue = ConvertToSdlKeybind(keybind);
    return std::make_unique<Binding>(std::move(command), keybindValue, triggerState);
#endif
}

#if _WIN32
int ControllerInput::Impl::ConvertToXInput(InputKeybinds keybind)
{
    assert(
        static_cast<int>(keybind) > static_cast<int>(InputKeybinds::CONTROLLER_BEGIN) &&
        static_cast<int>(keybind) < static_cast<int>(InputKeybinds::CONTROLLER_END) &&
        "Keybind has to be a controller keybind");
    // this works only if the things stay ordered and xinput does not change the layout
    return 1 << (static_cast<int>(keybind) - static_cast<int>(InputKeybinds::DPAD_UP));
}
#else
int ControllerInput::Impl::ConvertToSdlKeybind(InputKeybinds keybind)
{
    switch (keybind)
    {
    case InputKeybinds::DPAD_UP:
        return SDL_GAMEPAD_BUTTON_DPAD_UP;
    case InputKeybinds::DPAD_DOWN:
        return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
    case InputKeybinds::DPAD_LEFT:
        return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
    case InputKeybinds::DPAD_RIGHT:
        return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
    case InputKeybinds::BUTTON_EAST:
        return SDL_GAMEPAD_BUTTON_EAST;
    case InputKeybinds::BUTTON_SOUTH:
        return SDL_GAMEPAD_BUTTON_SOUTH;
    case InputKeybinds::BUTTON_WEST:
        return SDL_GAMEPAD_BUTTON_WEST;
    case InputKeybinds::START:
        return SDL_GAMEPAD_BUTTON_START;
    case InputKeybinds::BACK:
        return SDL_GAMEPAD_BUTTON_BACK;
    case InputKeybinds::LEFT_THUMB:
        return SDL_GAMEPAD_BUTTON_LEFT_STICK;
    case InputKeybinds::RIGHT_THUMB:
        return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
    case InputKeybinds::LEFT_SHOULDER:
        return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
    case InputKeybinds::RIGHT_SHOULDER:
        return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
    default:
        return -1;
    }
}
#endif
