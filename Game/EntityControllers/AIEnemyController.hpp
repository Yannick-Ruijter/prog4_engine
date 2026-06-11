#pragma once

#include "InputProvider.hpp"
#include "Observer.hpp"
#include <glm/glm.hpp>
#include <vector>
namespace dae {
class RectCollider;
class GameObject;
class Transform;
class GameManager;
class Subject;
class AIEnemyController : public InputProvider {
public:
  AIEnemyController(GameObject *controlledEnemy, GameManager *manager);
  virtual ~AIEnemyController() override;
  virtual glm::vec2 GetMovementDirection() override;
  virtual bool AttackButtonPressed() const override;
  virtual bool IsStunned() override;
  virtual bool ShouldDie() const override;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<Subject *> m_EventsToUnBind{};
  GameObject *m_ControlledEnemy{nullptr};
  Transform *m_Transform{nullptr};
  GameObject *m_TargetPlayer{nullptr};
  Transform *m_TargetTransform{nullptr};
  GameManager *m_Manager{nullptr};
  RectCollider *m_Collider{nullptr};
  glm::vec2 m_MovementVec{};
  float m_Speed{};
  bool m_HasBeenStunned{false};
  bool m_ShouldDie{false};
  bool m_WasOnCrossRoads{false};
  bool m_FirstLoop{true};
};
} // namespace dae
