#include "ScoreComponent.h"
#include "Subject.h"
#include "Event.h"
using namespace dae;

dae::ScoreComponent::ScoreComponent(GameObject& owner, std::unique_ptr<Subject>&& scoreChangedEvent)
	:Component(owner), m_ScoreChangedEvent{ std::move(scoreChangedEvent) }
{
}

void ScoreComponent::AddScore(int score)
{
	m_CurrentScore += score;
	m_ScoreChangedEvent->NotifyObservers(Event::ScoreChanged, GetOwner());
}

int dae::ScoreComponent::GetScore() const
{
	return m_CurrentScore;
}

void dae::ScoreComponent::Notify(Event event, GameObject*)
{
	if (event == Event::ItemPickedUp) AddScore(100);
}

Subject* dae::ScoreComponent::GetSubject() const
{
	return m_ScoreChangedEvent.get();
}
