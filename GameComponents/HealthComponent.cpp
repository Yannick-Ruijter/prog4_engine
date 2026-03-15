#include "HealthComponent.h"
#include "GameObject.h"

using namespace dae;
HealthComponent::HealthComponent(GameObject& owner, int numberOfLives)
	:Component{ owner }, m_NumberOfLives{ numberOfLives }
{}

void HealthComponent::Damage()
{
	m_NumberOfLives -= 1;
	//do notify thing
}

int dae::HealthComponent::GetNumberOfLives() const
{
	return m_NumberOfLives;
}
