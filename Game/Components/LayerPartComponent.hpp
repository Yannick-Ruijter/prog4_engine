#pragma once

#include "Component.hpp"
#include <vector>

namespace dae {
    class GameObject;
    class TransformComponent;
    class BurgerLayerComponent;
    class LayerPartComponent : public Component {
      public:
        static void SetCollisionDistance(int dist);
        static float SteppedOnOffset;
        LayerPartComponent(GameObject &owner, std::vector<GameObject *> const &players);
        virtual void Update() override;
        bool IsSteppedOn() const;
        void SetFallingState(bool state);

      private:
        static int CollisionDistanceSquared;
        bool m_IsSteppedOn{false};
        bool m_IsFalling{false};
        std::vector<TransformComponent *> m_PlayerTransforms{};
        TransformComponent *m_Transform{};
    };
} // namespace dae
