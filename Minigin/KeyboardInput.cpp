#include "Binding.hpp"
#include "Command.hpp"
#include "KeyboardInput.hpp"
#include <algorithm>

using namespace dae;
dae::KeyboardInput::KeyboardInput() = default;
dae::KeyboardInput::~KeyboardInput() = default;
void KeyboardInput::ProcessInput()
{
    if (m_PreviousState.get() == nullptr)
        m_PreviousState = std::make_unique<bool[]>(SDL_SCANCODE_COUNT);

    if (m_CurrentState != nullptr)
        std::memcpy(m_PreviousState.get(), m_CurrentState, SDL_SCANCODE_COUNT);
    // adds/removes bindings that were requested to be
    Updatebindings();
    SDL_PumpEvents();
    m_CurrentState = SDL_GetKeyboardState(nullptr);
    for (auto &binding : m_Bindings)
    {
        if (binding.get() && binding->m_TriggerState == InputState::JustPressed &&
            WasPressedThisFrame(binding->m_Keybind))
            binding->m_Command->Execute();
        if (binding.get() && binding->m_TriggerState == InputState::JustReleased &&
            WasReleasedThisFrame(binding->m_Keybind))
            binding->m_Command->Execute();
        if (binding.get() && binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind))
            binding->m_Command->Execute();
    }
}

bool dae::KeyboardInput::WasPressedThisFrame(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return WasPressedThisFrame(m_ActionsMapped.at(button));
}

bool dae::KeyboardInput::IsButtonPressed(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return IsButtonPressed(m_ActionsMapped.at(button));
}

bool dae::KeyboardInput::WasReleasedThisFrame(InputAction button) const
{
    assert(m_ActionsMapped.contains(button) && "This action has not been mapped to an input yet");
    return WasReleasedThisFrame(m_ActionsMapped.at(button));
}

bool KeyboardInput::WasPressedThisFrame(unsigned int button) const
{
    bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];

    return buttonChange && m_CurrentState[button];
}

bool KeyboardInput::IsButtonPressed(unsigned int button) const
{
    return m_CurrentState[button];
}

bool KeyboardInput::WasReleasedThisFrame(unsigned int button) const
{
    bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];

    return buttonChange && !m_CurrentState[button];
}

PlayerInput &dae::KeyboardInput::BindInputAction(InputAction action, InputKeybinds keybind)
{
    m_ActionsMapped[action] = ConvertToScancode(keybind);
    return *this;
}

Binding *KeyboardInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState)
{
    assert(command.get() != nullptr);
    int scancode = ConvertToScancode(keybind);

    // i save it here into this array to already be able to return the binding to the caller
    auto binding =
        m_BindingsToAdd.emplace_back(std::make_unique<Binding>(std::move(command), scancode, triggerState)).get();

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

void KeyboardInput::UnBind(Binding *binding)
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

int KeyboardInput::ConvertToScancode(InputKeybinds keybind)
{
    assert(
        static_cast<int>(keybind) > static_cast<int>(InputKeybinds::KEYBOARD_BEGIN) &&
        static_cast<int>(keybind) < static_cast<int>(InputKeybinds::KEYBOARD_END) &&
        "Keybind has to be a keyboard keybind");
    // only works for the chars in alphabeth but we don't need more for now
    return 4 + static_cast<int>(keybind) - static_cast<int>(InputKeybinds::A);
}

void dae::KeyboardInput::Updatebindings()
{
    while (!m_BindingCommandQueue.empty())
    {
        //it's a functoin
        m_BindingCommandQueue.front()();
        m_BindingCommandQueue.pop();
    }
}
