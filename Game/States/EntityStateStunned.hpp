#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
class RectCollider;
enum class Direction;
class EntityStateStunned : public EntityState {
public:
  EntityStateStunned(Entity &entity);
  ~EntityStateStunned();
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void OnEnter() override;
  virtual void OnExit() override;

private:
  Transform *m_EntityTransform;
  RectCollider *m_Collider{nullptr};
  float m_StopWatch{0.f};
  float m_StunnedTime{5.f};
};
} // namespace dae
