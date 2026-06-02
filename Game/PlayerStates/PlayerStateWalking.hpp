#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class Transform;
    class PlayerStateWalking : public PlayerState
    {
      public:
        PlayerStateWalking(PlayerController &player, Direction dir);
        ~PlayerStateWalking();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        Transform *m_PlayerTransform{nullptr};
        Direction m_CurrentMoveDir;
        glm::vec2 m_MovementVector{};
    };
} // namespace dae
