#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

void dae::TransformComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.transform = std::make_unique<TransformComponent>(parent);
}

dae::TransformComponent* dae::TransformComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.transform.get();
}

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