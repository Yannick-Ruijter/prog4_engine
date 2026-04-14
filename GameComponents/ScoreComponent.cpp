#include "ScoreComponent.h"
#include "Subject.h"
#include "sdbm_hash.h"
using namespace dae;

dae::ScoreComponent::ScoreComponent(GameObject& owner, std::unique_ptr<Subject>&& scoreChangedEvent)
	:Component(owner), m_ScoreChangedEvent{ std::move(scoreChangedEvent) }
{
}

void ScoreComponent::AddScore(int score)
{
	m_CurrentScore += score;
	m_ScoreChangedEvent->NotifyObservers("ScoreChanged"_h, GetOwner());
}

int dae::ScoreComponent::GetScore() const
{
	return m_CurrentScore;
}

void dae::ScoreComponent::Notify(unsigned int eventId, GameObject*)
{
	if (eventId == "ItemPickedUp"_h) AddScore(100);
}

Subject* dae::ScoreComponent::GetSubject() const
{
	return m_ScoreChangedEvent.get();
}
