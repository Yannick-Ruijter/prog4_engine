#pragma once
#include "Observer.h"
#include "Component.h"

namespace dae
{
	class HealthComponent;
	class TextComponent;
	enum class Event;
	class LivesDisplayComponent : public Component, public Observer
	{
	public:
		LivesDisplayComponent(GameObject& gameObject, HealthComponent& targetHealth);

		void Notify(unsigned int eventId, GameObject* source) override;
	private:
		HealthComponent* m_TargetHealth{ nullptr };
		TextComponent* m_TextComponent{ nullptr };
	};
}