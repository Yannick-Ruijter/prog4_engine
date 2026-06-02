#pragma once
#include "CustomCommands.hpp"
#include "PlayerState.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class Transform;
    class PlayerStateClimbing : public PlayerState
    {
      public:
        PlayerStateClimbing(Entity &player, Direction dir);
        ~PlayerStateClimbing();
        std::unique_ptr<PlayerState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        glm::vec2 m_MovementVector{};
        Direction m_CurrentMoveDir;
        Transform *m_PlayerTransform;
    };
} // namespace dae
