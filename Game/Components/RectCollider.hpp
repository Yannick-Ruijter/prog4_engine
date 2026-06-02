#pragma once
#include "Component.hpp"
#include "Subject.hpp"
#include <glm/glm.hpp>
#include <vector>
namespace dae {
    struct Rect {
        glm::vec2 topLeft;
        glm::vec2 dimensions;
    };

    class RectCollider : public Component, public Subject {
      public:
        RectCollider(GameObject &owner);
        const Rect &GetRect() const;
        void LateUpdate();
        RectCollider *GetLastCollision() const;

      private:
        Rect m_Rect{};
        RectCollider *m_LastCollision{nullptr};
        static std::vector<RectCollider *> m_Colliders;
    };
} // namespace dae
