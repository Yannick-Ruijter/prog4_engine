#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
class EntityStateClimbing : public EntityState {
public:
  EntityStateClimbing(Entity &entity, Direction dir);
  ~EntityStateClimbing();
  std::unique_ptr<EntityState> HandleInput();
  void Update();
  void OnEnter();
  void OnExit();

private:
  glm::vec2 m_MovementVector{};
  Direction m_CurrentMoveDir;
  Transform *m_EntityTransform;
};
} // namespace dae
