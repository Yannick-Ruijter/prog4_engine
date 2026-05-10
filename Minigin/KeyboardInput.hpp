#pragma once
#include "InputInfo.hpp"
#include "PlayerInput.hpp"
#include <SDL3/SDL.h>
#include <array>
#include <memory>
#include <vector>

namespace dae
{
    class Command;
    class Binding;
    class KeyboardInput : public PlayerInput
    {
      public:
        KeyboardInput();
        ~KeyboardInput();
        void ProcessInput() override;

        bool WasPressedThisFrame(InputKeybinds) const override;
        bool IsButtonPressed(InputKeybinds) const override;
        bool WasReleasedThisFrame(InputKeybinds) const override;

        bool WasPressedThisFrame(unsigned int button) const override;
        bool IsButtonPressed(unsigned int button) const override;
        bool WasReleasedThisFrame(unsigned int button) const override;

        Binding *AddBinding(
            std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState,
            InputState endTriggerState = InputState::None) override;
        std::unique_ptr<Binding> UnBind(Binding *binding) override;

      private:
        int ConvertToScancode(InputKeybinds keybind);

        const bool *m_CurrentState{nullptr};
        std::unique_ptr<bool[]> m_PreviousState{};

        std::vector<std::unique_ptr<Binding>> m_Bindings;
    };
} // namespace dae
