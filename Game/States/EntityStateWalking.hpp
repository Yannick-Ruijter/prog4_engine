#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae {
class Transform;
class EntityStateWalking : public EntityState {
public:
  EntityStateWalking(Entity &entity, Direction dir);
  virtual ~EntityStateWalking();
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void Update() override;
  virtual void OnEnter() override;
  virtual void OnExit() override;

private:
  Transform *m_EntityTransform{nullptr};
  Direction m_CurrentMoveDir;
  glm::vec2 m_MovementVector{};
};
} // namespace dae
