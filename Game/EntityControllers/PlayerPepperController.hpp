#pragma once

#include "InputProvider.hpp"

namespace dae {
class PlayerInput;
class PlayerPepperController : public InputProvider {
public:
  PlayerPepperController(PlayerInput *input);
  virtual ~PlayerPepperController() override;
  virtual glm::vec2 GetMovementDirection() const override;
  virtual bool AttackButtonPressed() const override;

private:
  PlayerInput *m_Input{nullptr};
};
} // namespace dae
