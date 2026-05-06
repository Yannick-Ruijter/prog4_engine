#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
namespace dae
{
    class PlayerStateClimbing : public PlayerState
    {
      public:
        PlayerStateClimbing(PlayerComponent &player, MoveDirection dir);
        ~PlayerStateClimbing();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();
        void Notify(unsigned int eventId, GameObject *source);

      private:
        bool m_PlayerStoppedClimbing{false};
        MoveDirection m_CurrentMoveDir;
    };
} // namespace dae
