#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
enum class Direction;
class EntityStateAttacking : public EntityState {
public:
  EntityStateAttacking(Entity &entity, Direction dir);
  virtual ~EntityStateAttacking();
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void Update() override;
  virtual void OnEnter() override;
  virtual void OnExit() override;

private:
  glm::vec2 m_MovementVector{};
  Direction m_CurrentFaceDir;
  Transform *m_EntityTransform;
  float m_StopWatch{0.f};
  float m_AttackTime{1.f};
  GameObject *m_Pepper;

  Direction VecToDirection(glm::vec2 const &vec);
  glm::vec2 DirectionToVec(Direction dir);
  void SetAnimationState();
  void CreatePepper();
};
} // namespace dae
