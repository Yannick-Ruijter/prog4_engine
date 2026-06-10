#pragma once

#include "InputProvider.hpp"
#include "Observer.hpp"
#include <vector>
namespace dae {
class PlayerInput;
class RectCollider;
class GameObject;
class PlayerEnemyController : public InputProvider {
public:
  PlayerEnemyController(std::vector<PlayerInput *> inputs,
                        GameObject *controlledEnemy);
  virtual ~PlayerEnemyController() override;
  virtual glm::vec2 GetMovementDirection() const override;
  virtual bool AttackButtonPressed() const override;
  virtual bool IsStunned() override;
  virtual bool ShouldDie() const override;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<PlayerInput *> m_Inputs{nullptr};
  GameObject *m_ControlledEnemy{nullptr};
  RectCollider *m_Collider{nullptr};
  bool m_HasBeenStunned{false};
  bool m_ShouldDie{false};
};
} // namespace dae
