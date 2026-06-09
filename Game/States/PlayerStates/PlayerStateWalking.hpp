#pragma once
#include "CustomCommands.hpp"
#include "EntityState.hpp"
#include <glm/glm.hpp>
namespace dae
{
    class Transform;
    class PlayerStateWalking : public EntityState
    {
      public:
        PlayerStateWalking(Entity &player, Direction dir);
        ~PlayerStateWalking();
        std::unique_ptr<EntityState> HandleInput();
        void Update();
        void OnEnter();
        void OnExit();

      private:
        Transform *m_PlayerTransform{nullptr};
        Direction m_CurrentMoveDir;
        glm::vec2 m_MovementVector{};
    };
} // namespace dae
