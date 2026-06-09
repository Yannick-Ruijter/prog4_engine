#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
class EntityStateWalking : public EntityState {
public:
  EntityStateWalking(Entity &entity, Direction dir);
  ~EntityStateWalking();
  std::unique_ptr<EntityState> HandleInput();
  void Update();
  void OnEnter();
  void OnExit();

private:
  Transform *m_EntityTransform{nullptr};
  Direction m_CurrentMoveDir;
  glm::vec2 m_MovementVector{};
};
} // namespace dae
