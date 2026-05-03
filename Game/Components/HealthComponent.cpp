#include "HealthComponent.hpp"
#include "GameObject.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
using namespace dae;
HealthComponent::HealthComponent(GameObject& owner, std::unique_ptr<Subject>&& liveLostEvent, int numberOfLives)
	:Component{ owner }, m_NumberOfLives{ numberOfLives }, m_LiveLostEvent{std::move(liveLostEvent)}
{
}

dae::HealthComponent::~HealthComponent() = default;

void HealthComponent::Damage()
{

	m_NumberOfLives -= 1;
	if(m_LiveLostEvent.get())
		m_LiveLostEvent->NotifyObservers("PlayerDeath"_h, GetOwner());
}

int dae::HealthComponent::GetNumberOfLives() const
{
	return m_NumberOfLives;
}

Subject* dae::HealthComponent::GetSubject() const
{
	return m_LiveLostEvent.get();
}
