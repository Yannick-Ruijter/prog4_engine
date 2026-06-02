#include "GameObject.hpp"
#include "Health.hpp"
#include "LivesDisplay.hpp"
#include "TextDisplay.hpp"
#include "sdbm_hash.hpp"

dae::LivesDisplay::LivesDisplay(GameObject &gameObject, Health &targetHealth)
    : Component(gameObject), m_TargetHealth{&targetHealth} {
    // force event to update the text
    Notify("PlayerDeath"_h, &gameObject);
}

void dae::LivesDisplay::Notify(EventId eventId, GameObject *) {
    if (m_TextComponent == nullptr)
        m_TextComponent = GetOwner()->GetComponent<dae::TextDisplay>();
    assert(m_TextComponent != nullptr);
    if (eventId == "PlayerDeath"_h) {
        int currentHealth = m_TargetHealth->GetNumberOfLives();
        m_TextComponent->SetText("# Lives: " + std::to_string(currentHealth));
    }
}
