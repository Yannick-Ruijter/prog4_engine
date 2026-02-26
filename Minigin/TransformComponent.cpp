#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject& owner, glm::vec3 const& position) : Component(owner), m_LocalPosition{ position }, m_WorldPosition{ position } {}

void dae::TransformComponent::Update(float) {}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
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

void dae::TransformComponent::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	m_UpdateWorldPos = true;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position) 
{
	SetLocalPosition(position.x, position.y, position.z);
}
