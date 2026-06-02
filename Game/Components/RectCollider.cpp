#include "GameObject.hpp"
#include "RectCollider.hpp"
#include "sdbm_hash.hpp"

using namespace dae;
std::vector<RectCollider *> RectCollider::m_Colliders = {};
RectCollider::RectCollider(GameObject &owner) : Component(owner) {
    m_Colliders.push_back(this);
}

const Rect &dae::RectCollider::GetRect() const {
    return m_Rect;
}

void dae::RectCollider::LateUpdate() {
    for (auto const &collider : m_Colliders) {
        if (collider == this)
            continue;

        auto otherRect = collider->GetRect();

        if ((otherRect.topLeft.x + otherRect.dimensions.x) < m_Rect.topLeft.x ||
            otherRect.topLeft.x > (m_Rect.topLeft.x + m_Rect.dimensions.x))
            continue;
        if ((otherRect.topLeft.y + otherRect.dimensions.y) < m_Rect.topLeft.y ||
            otherRect.topLeft.y > (m_Rect.topLeft.y + m_Rect.dimensions.y))
            continue;

        m_LastCollision = collider;
        NotifyObservers("OnCollision"_h, GetOwner());
    }
}

RectCollider *dae::RectCollider::GetLastCollision() const {
    return m_LastCollision;
}
