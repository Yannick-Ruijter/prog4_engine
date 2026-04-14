#include "LivesDisplayComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "sdbm_hash.h"

dae::LivesDisplayComponent::LivesDisplayComponent(GameObject& gameObject, HealthComponent& targetHealth)
	:Component(gameObject), m_TargetHealth{&targetHealth}
{
	//force event to update the text
	Notify("PlayerDeath"_h, &gameObject);
}

void dae::LivesDisplayComponent::Notify(unsigned int eventId, GameObject*)
{
	if(m_TextComponent == nullptr) m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	assert(m_TextComponent != nullptr);
	if (eventId == "PlayerDeath"_h)
	{
		int currentHealth = m_TargetHealth->GetNumberOfLives();
		m_TextComponent->SetText("# Lives: " + std::to_string(currentHealth));
	}
}
