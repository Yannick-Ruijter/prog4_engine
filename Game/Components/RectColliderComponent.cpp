#include "RectColliderComponent.hpp"
#include "GameObject.hpp"
#include "sdbm_hash.hpp"

using namespace dae;
std::vector<RectColliderComponent *> RectColliderComponent::m_Colliders = {};
RectColliderComponent::RectColliderComponent(GameObject &owner) : Component(owner)
{
    m_Colliders.push_back(this);
}

const Rect &dae::RectColliderComponent::GetRect() const
{
    return m_Rect;
}

void dae::RectColliderComponent::LateUpdate()
{
    for (auto const &collider : m_Colliders)
    {
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

RectColliderComponent *dae::RectColliderComponent::GetLastCollision() const
{
    return m_LastCollision;
}
