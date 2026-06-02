#include "GameObject.hpp"
#include "Health.hpp"
#include "PlayerLivesObserver.hpp"
#include "TextDisplay.hpp"
#include "sdbm_hash.hpp"
#include <string>
using namespace dae;

dae::PlayerLivesObserver::PlayerLivesObserver(TextDisplay *textComponent) : m_TargetTextComponent{textComponent}
{
}

void PlayerLivesObserver::Notify(EventId eventId, GameObject *source)
{
    if (eventId == "PlayerDeath"_h)
    {
        Health *healthComponent = source->GetComponent<Health>();
        std::string displayText{"# Lives: " + std::to_string(healthComponent->GetNumberOfLives())};
        m_TargetTextComponent->SetText(displayText);
    }
}
