#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
namespace dae {
class EntityStateIdle : public EntityState {
public:
  EntityStateIdle(Entity &entity, Direction direction = Direction::Down);
  ~EntityStateIdle();
  std::unique_ptr<EntityState> HandleInput();
  void Update();
  void OnEnter();
  void OnExit();

private:
  Direction m_CurrentFacingDir;
};
} // namespace dae
