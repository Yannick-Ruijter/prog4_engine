#include "GameObject.hpp"
#include "HealthComponent.hpp"
#include "PlayerLivesObserver.hpp"
#include "TextComponent.hpp"
#include "sdbm_hash.hpp"
#include <string>
using namespace dae;

dae::PlayerLivesObserver::PlayerLivesObserver(TextComponent *textComponent) : m_TargetTextComponent{textComponent}
{
}

void PlayerLivesObserver::Notify(EventId eventId, GameObject *source)
{
    if (eventId == "PlayerDeath"_h)
    {
        HealthComponent *healthComponent = source->GetComponent<HealthComponent>();
        std::string displayText{"# Lives: " + std::to_string(healthComponent->GetNumberOfLives())};
        m_TargetTextComponent->SetText(displayText);
    }
}
