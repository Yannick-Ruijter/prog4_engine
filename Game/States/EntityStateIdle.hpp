#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
namespace dae {
class EntityStateIdle : public EntityState {
public:
  EntityStateIdle(Entity &entity, Direction direction = Direction::Down);
  virtual ~EntityStateIdle();
  virtual std::unique_ptr<EntityState> HandleInput() override;
  virtual void Update() override;
  virtual void OnEnter() override;
  virtual void OnExit() override;

private:
  Direction m_CurrentFacingDir;
  Direction m_PreviousDirection;
};
} // namespace dae
