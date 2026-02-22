#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject& owner, glm::vec3 const& position) : Component(owner), m_Position{ position } {}

void dae::TransformComponent::Update(float) {}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::TransformComponent::SetPosition(const glm::vec3& position) 
{ 
	m_Position = position;
}