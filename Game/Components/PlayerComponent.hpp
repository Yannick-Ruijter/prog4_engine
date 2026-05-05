#pragma once

#include "Component.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae
{
    class PlayerAnimationComponent;
    class PlayerState;
    class PlayerComponent final : public Component, public Observer
    {
      public:
        PlayerComponent(GameObject &owner);
        ~PlayerComponent();
        void Update();
        GameObject *GetPlayer();
        void Notify(unsigned int eventId, GameObject *source);
        PlayerAnimationComponent *GetPlayerAnimation() const;

      private:
        PlayerAnimationComponent *m_PlayerAnimation;
        std::unique_ptr<PlayerState> m_CurrentState;
    };
} // namespace dae
