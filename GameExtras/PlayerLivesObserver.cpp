#include "PlayerLivesObserver.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include <string>
#include "sdbm_hash.h"
using namespace dae;

dae::PlayerLivesObserver::PlayerLivesObserver(TextComponent* textComponent)
	:m_TargetTextComponent{textComponent}
{
}

void PlayerLivesObserver::Notify(unsigned int eventId, GameObject* source)
{
	if (eventId == "PlayerDeath"_h)
	{
		HealthComponent* healthComponent = source->GetComponent<HealthComponent>();
		std::string displayText{ "# Lives: " + std::to_string(healthComponent->GetNumberOfLives()) };
		m_TargetTextComponent->SetText(displayText);
	}
}