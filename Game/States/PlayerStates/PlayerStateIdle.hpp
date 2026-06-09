#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
namespace dae {
class PlayerStateIdle : public EntityState {
public:
  PlayerStateIdle(Entity &player, Direction direction = Direction::Down);
  ~PlayerStateIdle();
  std::unique_ptr<EntityState> HandleInput();
  void Update();
  void OnEnter();
  void OnExit();

private:
  Direction m_CurrentFacingDir;
};
} // namespace dae
