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
        void Notify(unsigned int eventId, GameObject *source);

      private:
        bool m_StartedMoving{false};
        bool m_StartedClimbing{false};
        MoveDirection m_CurrentFacingDir;
    };
} // namespace dae
