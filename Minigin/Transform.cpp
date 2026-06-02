#include "GameObject.hpp"
#include "Transform.hpp"
#include <memory>

dae::Transform::Transform(GameObject &owner, glm::vec2 const &position)
    : Component(owner), m_LocalPosition{position}, m_WorldPosition{position} {
}

void dae::Transform::Update() {
}

const glm::vec2 &dae::Transform::GetWorldPosition() {
    if (m_UpdateWorldPos) {
        if (GetOwner()->GetParent() != nullptr) {
            m_WorldPosition = GetOwner()->GetParent()->GetWorldPosition() + m_LocalPosition;
        } else {
            m_WorldPosition = m_LocalPosition;
        }
        m_UpdateWorldPos = false;
    }
    return m_WorldPosition;
}

void dae::Transform::SetLocalPosition(const float x, const float y) {
    m_LocalPosition.x = x;
    m_LocalPosition.y = y;
    m_UpdateWorldPos = true;
}

void dae::Transform::SetLocalPosition(const glm::vec2 &position) {
    SetLocalPosition(position.x, position.y);
}
