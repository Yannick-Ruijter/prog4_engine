#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Event.h"
#include "GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject& gameObject, ScoreComponent& targetScore)
	:Component(gameObject), m_TargetScore{ &targetScore }
{
	//force event to update the text
	Notify(Event::PlayerDeath, &gameObject);
}

void dae::ScoreDisplayComponent::Notify(Event event, GameObject*)
{
	if (m_TextComponent == nullptr) m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	assert(m_TextComponent != nullptr);
	if (event == Event::ScoreChanged)
	{
		int currentScore = m_TargetScore->GetScore();
		m_TextComponent->SetText("Score: " + std::to_string(currentScore));
	}
}
