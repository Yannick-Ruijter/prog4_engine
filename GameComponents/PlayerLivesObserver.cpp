#pragma once

#include "PlayerLivesObserver.h"
#include "Event.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include <string>
using namespace dae;

dae::PlayerLivesObserver::PlayerLivesObserver(TextComponent* textComponent)
	:m_TargetTextComponent{textComponent}
{
}

void PlayerLivesObserver::Notify(Event event, GameObject* source)
{
	if (event == Event::PlayerDeath)
	{
		HealthComponent* healthComponent = source->GetComponent<HealthComponent>();
		std::string displayText{ "# Lives: " + std::to_string(healthComponent->GetNumberOfLives()) };
		m_TargetTextComponent->SetText(displayText);
	}
}