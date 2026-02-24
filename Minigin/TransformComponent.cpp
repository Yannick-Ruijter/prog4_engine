#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject& owner, glm::vec3 const& position) : Component(owner), m_WorldPosition{ position } {}

void dae::TransformComponent::Update(float) {}

void dae::TransformComponent::SetWorldPosition(const float x, const float y, const float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
}

void dae::TransformComponent::SetWorldPosition(const glm::vec3& position) 
{
	m_WorldPosition = position;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
}
