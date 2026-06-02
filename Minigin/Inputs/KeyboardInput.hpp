#pragma once
#include "InputInfo.hpp"
#include "PlayerInput.hpp"
#include <SDL3/SDL.h>
#include <array>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <vector>

namespace dae
{
    class Command;
    class Binding;
    class KeyboardInput final : public PlayerInput
    {
      public:
        KeyboardInput();
        ~KeyboardInput();
        void ProcessInput() override;

        bool WasPressedThisFrame(InputAction button) const override;
        bool IsButtonPressed(InputAction button) const override;
        bool WasReleasedThisFrame(InputAction button) const override;

        bool WasPressedThisFrame(unsigned int button) const override;
        bool IsButtonPressed(unsigned int button) const override;
        bool WasReleasedThisFrame(unsigned int button) const override;

        PlayerInput &BindInputAction(InputAction action, InputKeybinds keybind) override;

        Binding *AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState) override;
        void UnBind(Binding *binding) override;

      private:
        int ConvertToScancode(InputKeybinds keybind);
        void Updatebindings();

        const bool *m_CurrentState{nullptr};
        std::unique_ptr<bool[]> m_PreviousState{};

        std::vector<std::unique_ptr<Binding>> m_Bindings;
        std::vector<std::unique_ptr<Binding>> m_BindingsToAdd;
        std::queue<std::function<void()>> m_BindingCommandQueue{};
        std::map<InputAction, unsigned int> m_ActionsMapped{};
    };
} // namespace dae
