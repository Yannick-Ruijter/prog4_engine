#include "GameObject.hpp"
#include "ScoreComponent.hpp"
#include "ScoreDisplayComponent.hpp"
#include "TextComponent.hpp"
#include "sdbm_hash.hpp"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject &gameObject, ScoreComponent &targetScore)
    : Component(gameObject), m_TargetScore{&targetScore}
{
    // force event to update the text
    Notify("PlayerDeath"_h, &gameObject);
}

void dae::ScoreDisplayComponent::Notify(EventId eventId, GameObject *)
{
    if (m_TextComponent == nullptr)
        m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();
    assert(m_TextComponent != nullptr);
    if (eventId == "ScoreChanged"_h)
    {
        int currentScore = m_TargetScore->GetScore();
        m_TextComponent->SetText("Score: " + std::to_string(currentScore));
    }
}
