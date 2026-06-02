#include "GameObject.hpp"
#include "Score.hpp"
#include "ScoreDisplay.hpp"
#include "TextDisplay.hpp"
#include "sdbm_hash.hpp"

dae::ScoreDisplay::ScoreDisplay(GameObject &gameObject, Score &targetScore)
    : Component(gameObject), m_TargetScore{&targetScore} {
    // force event to update the text
    Notify("PlayerDeath"_h, &gameObject);
}

void dae::ScoreDisplay::Notify(EventId eventId, GameObject *) {
    if (m_TextComponent == nullptr)
        m_TextComponent = GetOwner()->GetComponent<dae::TextDisplay>();
    assert(m_TextComponent != nullptr);
    if (eventId == "ScoreChanged"_h) {
        int currentScore = m_TargetScore->GetScore();
        m_TextComponent->SetText("Score: " + std::to_string(currentScore));
    }
}
