#pragma once
#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Event.h"
#include "GameObject.h"

dae::LivesDisplayComponent::LivesDisplayComponent(GameObject& gameObject, HealthComponent& targetHealth)
	:Component(gameObject), m_TargetHealth{&targetHealth}
{
	//force event to update the text
	Notify(Event::PlayerDeath, &gameObject);
}

void dae::LivesDisplayComponent::Notify(Event event, GameObject*)
{
	if(m_TextComponent == nullptr) m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	assert(m_TextComponent != nullptr);
	if (event == Event::PlayerDeath)
	{
		int currentHealth = m_TargetHealth->GetNumberOfLives();
		m_TextComponent->SetText("# Lives: " + std::to_string(currentHealth));
	}
}
