#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
namespace dae
{
    class PlayerStateIdle : public PlayerState
    {
      public:
        PlayerStateIdle(PlayerComponent &player, MoveDirection direction = MoveDirection::Down);
        ~PlayerStateIdle();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        MoveDirection m_CurrentFacingDir;
    };
} // namespace dae
