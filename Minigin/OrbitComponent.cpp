#define _USE_MATH_DEFINES
#include "OrbitComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include <cmath>

dae::OrbitComponent::OrbitComponent(GameObject& owner, float orbitOffset, float orbitSpeed)
	:Component(owner), m_OrbitOffset{orbitOffset}, m_OrbitSpeed{orbitSpeed}
{

}

void dae::OrbitComponent::Update(float deltaTime)
{
	m_CurrentAngle += m_OrbitSpeed * deltaTime - static_cast<float>((m_CurrentAngle > M_PI * 2) * M_PI * 2);
	glm::vec3 currentPos{ m_OrbitOffset * cos(m_CurrentAngle), m_OrbitOffset * sin(m_CurrentAngle), 0.f };
	GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(currentPos);
}
