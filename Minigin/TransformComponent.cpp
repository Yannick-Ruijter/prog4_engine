#include "TransformComponent.h"
#include <memory>
#include "GameObject.h"

void dae::TransformComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.transform = std::make_unique<TransformComponent>();
}

dae::TransformComponent* dae::TransformComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.transform.get();
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::TransformComponent::SetPosition(const glm::vec3& position) 
{ 
	m_position = position; 
}