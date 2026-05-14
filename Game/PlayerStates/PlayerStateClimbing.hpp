#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class TransformComponent;
    class PlayerStateClimbing : public PlayerState
    {
      public:
        PlayerStateClimbing(PlayerComponent &player, MoveDirection dir);
        ~PlayerStateClimbing();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        glm::vec2 m_MovementVector{};
        MoveDirection m_CurrentMoveDir;
        TransformComponent *m_PlayerTransform;
    };
} // namespace dae
