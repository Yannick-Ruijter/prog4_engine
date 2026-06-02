#pragma once

#include "Component.hpp"
#include <vector>

namespace dae {
    class GameObject;
    class Transform;
    class BurgerLayer;
    class BurgerLayerPart : public Component {
      public:
        static void SetCollisionDistance(int dist);
        static float SteppedOnOffset;
        BurgerLayerPart(GameObject &owner, std::vector<GameObject *> const &players);
        virtual void Update() override;
        bool IsSteppedOn() const;
        void SetFallingState(bool state);

      private:
        static int CollisionDistanceSquared;
        bool m_IsSteppedOn{false};
        bool m_IsFalling{false};
        std::vector<Transform *> m_PlayerTransforms{};
        Transform *m_Transform{};
    };
} // namespace dae
