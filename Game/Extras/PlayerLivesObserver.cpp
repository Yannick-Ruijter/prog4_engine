#include "PlayerLivesObserver.hpp"
#include "GameObject.hpp"
#include "TextComponent.hpp"
#include "HealthComponent.hpp"
#include <string>
#include "sdbm_hash.hpp"
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