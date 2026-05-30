#pragma once

#include "Component.hpp"
#include <vector>

namespace dae {
    class GameObject;
    class TransformComponent;
    class LayerPartComponent : public Component {
      public:
        static void SetCollisionDistance(int dist);
        LayerPartComponent(GameObject &owner, std::vector<GameObject *> const &players);
        virtual void Update() override;
        bool IsSteppedOn() const;
        void Startfalling() const;

      private:
        static int CollisionDistanceSquared;
        bool m_IsSteppedOn{false};
        std::vector<TransformComponent *> m_PlayerTransforms{};
        TransformComponent *m_Transform{};
    };
} // namespace dae
