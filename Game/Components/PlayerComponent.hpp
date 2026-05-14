#pragma once

#include "Component.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae
{
    class PlayerAnimationComponent;
    class PlayerState;
    class PlayerInput;
    class LevelGridComponent;
    class PlayerComponent final : public Component
    {
      public:
        PlayerComponent(GameObject &owner, PlayerInput *input, LevelGridComponent *level);
        ~PlayerComponent();
        void Update();
        GameObject *GetPlayer();
        PlayerAnimationComponent *GetPlayerAnimation() const;
        PlayerInput *GetInput() const;
        LevelGridComponent *GetLevel() const;

      private:
        PlayerAnimationComponent *m_PlayerAnimation;
        std::unique_ptr<PlayerState> m_CurrentState;
        PlayerInput *m_PlayerInput;
        LevelGridComponent *m_Level;
    };
} // namespace dae
