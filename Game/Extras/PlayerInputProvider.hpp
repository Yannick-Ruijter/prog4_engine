#pragma once

#include "InputProvider.hpp"

namespace dae {
    class PlayerInput;
    class PlayerInputProvider : public InputProvider {
      public:
        PlayerInputProvider(PlayerInput *input);
        virtual ~PlayerInputProvider() override;
        virtual glm::vec2 GetMovementDirection() const override;
        virtual bool AttackButtonPressed() const override;

      private:
        PlayerInput *m_Input{nullptr};
    };
} // namespace dae
