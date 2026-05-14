#include "TransformComponent.hpp"
#include "GameObject.hpp"
#include <memory>

dae::TransformComponent::TransformComponent(GameObject &owner, glm::vec2 const &position)
    : Component(owner), m_LocalPosition{position}, m_WorldPosition{position}
{
}

void dae::TransformComponent::Update()
{
}

const glm::vec2 &dae::TransformComponent::GetWorldPosition()
{
    if (m_UpdateWorldPos)
    {
        if (GetOwner()->GetParent() != nullptr)
        {
            m_WorldPosition = GetOwner()->GetParent()->GetWorldPosition() + m_LocalPosition;
        }
        else
        {
            m_WorldPosition = m_LocalPosition;
        }
        m_UpdateWorldPos = false;
    }
    return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(const float x, const float y)
{
    m_LocalPosition.x = x;
    m_LocalPosition.y = y;
    m_UpdateWorldPos = true;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec2 &position)
{
    SetLocalPosition(position.x, position.y);
}
