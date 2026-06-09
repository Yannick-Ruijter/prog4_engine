#pragma once
#include <glm/glm.hpp>

namespace dae {
    class PlayerInput;
    class InputProvider {
      public:
        virtual ~InputProvider() {};
        virtual glm::vec2 GetMovementDirection() const = 0;
        virtual bool AttackButtonPressed() const = 0;
    };
} // namespace dae
