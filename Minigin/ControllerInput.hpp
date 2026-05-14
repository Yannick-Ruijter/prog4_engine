#pragma once
#include "InputInfo.hpp"
#include "PlayerInput.hpp"
#include <memory>
#include <vector>

namespace dae
{
    class Command;
    class Binding;
    class ControllerInput final : public PlayerInput
    {
      public:
        ControllerInput(int controllerIndex);
        ~ControllerInput();
        void ProcessInput() override;

        bool WasPressedThisFrame(InputAction button) const override;
        bool IsButtonPressed(InputAction button) const override;
        bool WasReleasedThisFrame(InputAction button) const override;

        bool WasPressedThisFrame(unsigned int button) const override;
        bool IsButtonPressed(unsigned int button) const override;
        bool WasReleasedThisFrame(unsigned int button) const override;

        PlayerInput &BindInputAction(InputAction action, InputKeybinds keybind) override;

        Binding *AddBinding(
            std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState,
            InputState endTriggerState = InputState::None) override;
        std::unique_ptr<Binding> UnBind(Binding *binding) override;

      private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
        std::vector<std::unique_ptr<Binding>> m_Bindings;
    };
} // namespace dae
