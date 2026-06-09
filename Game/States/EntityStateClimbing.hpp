#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
class EntityStateClimbing : public EntityState {
public:
  EntityStateClimbing(Entity &entity, Direction dir);
  virtual ~EntityStateClimbing();
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void Update() override;
  virtual void OnEnter() override;
  virtual void OnExit() override;

private:
  glm::vec2 m_MovementVector{};
  Direction m_CurrentMoveDir;
  Transform *m_EntityTransform;
};
} // namespace dae
