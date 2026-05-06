#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
namespace dae
{
    class PlayerStateWalking : public PlayerState
    {
      public:
        PlayerStateWalking(PlayerComponent &player, MoveDirection dir);
        ~PlayerStateWalking();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();
        void Notify(unsigned int eventId, GameObject *source);

      private:
        bool m_StoppedMoving{false};
        MoveDirection m_CurrentMoveDir;
    };
} // namespace dae
