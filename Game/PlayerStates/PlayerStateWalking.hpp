#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class TransformComponent;
    class PlayerStateWalking : public PlayerState
    {
      public:
        PlayerStateWalking(PlayerComponent &player, Direction dir);
        ~PlayerStateWalking();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        TransformComponent *m_PlayerTransform{nullptr};
        Direction m_CurrentMoveDir;
        glm::vec2 m_MovementVector{};
    };
} // namespace dae
