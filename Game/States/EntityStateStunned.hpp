#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
enum class Direction;
class EntityStateStunned : public EntityState {
public:
  EntityStateStunned(Entity &entity);
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void OnEnter() override;

private:
  Transform *m_EntityTransform;
  float m_StopWatch{0.f};
  float m_StunnedTime{5.f};
};
} // namespace dae
