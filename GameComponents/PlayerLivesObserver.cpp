#pragma once

#include "PlayerLivesObserver.h"
#include "Event.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include <string>
using namespace dae;

void PlayerLivesObserver::Notify(Event event, GameObject* object)
{
	if (event == Event::PlayerDeath)
	{
		TextComponent* textComponent = object->GetComponent<TextComponent>();
		HealthComponent* healthComponent = object->GetComponent<HealthComponent>();
		std::string displayText{ "# Lives: " + std::to_string(healthComponent->GetNumberOfLives()) };
		textComponent->SetText(displayText);
	}
}