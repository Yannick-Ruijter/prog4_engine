#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
namespace dae
{
    class PlayerStateIdle : public PlayerState
    {
      public:
        PlayerStateIdle(Entity &player, Direction direction = Direction::Down);
        ~PlayerStateIdle();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        Direction m_CurrentFacingDir;
    };
} // namespace dae
