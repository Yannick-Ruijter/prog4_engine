#pragma once
#include "Observer.hpp"
#include "Component.hpp"

namespace dae
{
	class ScoreComponent;
	class TextComponent;
	enum class Event;
	class ScoreDisplayComponent : public Component, public Observer
	{
	public:
		ScoreDisplayComponent(GameObject& gameObject, ScoreComponent& targetHealth);

		void Notify(unsigned int eventId, GameObject* source) override;
	private:
		ScoreComponent* m_TargetScore{ nullptr };
		TextComponent* m_TextComponent{ nullptr };
	};
}