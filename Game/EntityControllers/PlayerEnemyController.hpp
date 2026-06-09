#pragma once

#include "InputProvider.hpp"

namespace dae {
class PlayerInput;
class PlayerEnemyController : public InputProvider {
public:
  PlayerEnemyController(PlayerInput *input);
  virtual ~PlayerEnemyController() override;
  virtual glm::vec2 GetMovementDirection() const override;
  virtual bool AttackButtonPressed() const override;

private:
  PlayerInput *m_Input{nullptr};
};
} // namespace dae
