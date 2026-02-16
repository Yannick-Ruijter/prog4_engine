#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* parent) : Component(parent), m_Position{} {}

void dae::TransformComponent::Update(float deltaTime)
{
	deltaTime;
}

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