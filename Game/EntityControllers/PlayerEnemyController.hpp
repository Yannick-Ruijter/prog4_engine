#pragma once

#include "InputProvider.hpp"
#include "Observer.hpp"
namespace dae {
class PlayerInput;
class RectCollider;
class GameObject;
class PlayerEnemyController : public InputProvider {
public:
  PlayerEnemyController(PlayerInput *input, GameObject *controlledEnemy);
  virtual ~PlayerEnemyController() override;
  virtual glm::vec2 GetMovementDirection() const override;
  virtual bool AttackButtonPressed() const override;

  void Notify(EventId eventId, GameObject *source) override;

private:
  PlayerInput *m_Input{nullptr};
  GameObject *m_ControlledEnemy{nullptr};
  RectCollider *m_Collider{nullptr};
};
} // namespace dae
