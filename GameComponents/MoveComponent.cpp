#include "MoveComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace dae;
MoveComponent::MoveComponent(GameObject& owner, float speed)
	:Component(owner), m_TransformComponent(owner.GetComponent<TransformComponent>()), m_Speed{speed}
{}

void dae::MoveComponent::Update(float deltaTime)
{
	if(m_ShouldMove)
	{
		m_TransformComponent->SetLocalPosition(m_TransformComponent->GetLocalPosition() + (m_DisplacementBuffer * deltaTime));
		m_DisplacementBuffer = glm::vec3{};
		m_ShouldMove = false;
	}
}

void dae::MoveComponent::Move(MoveDirection direction)
{
	if(direction == MoveDirection::Left) m_DisplacementBuffer.x -= m_Speed;
	else if(direction == MoveDirection::Right) m_DisplacementBuffer.x += m_Speed;
	else if(direction == MoveDirection::Up) m_DisplacementBuffer.y -= m_Speed;
	else if(direction == MoveDirection::Down) m_DisplacementBuffer.y += m_Speed;
	m_ShouldMove = true;
}
