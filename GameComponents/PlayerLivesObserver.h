#pragma once

#include "Observer.h"


enum class Event;
namespace dae
{
	class GameObject;
	class TextComponent;
	class PlayerLivesObserver : public Observer
	{
	public:
		PlayerLivesObserver(TextComponent* textComponent);
		~PlayerLivesObserver() override = default;
		void Notify(Event event, GameObject* source) override;
	private:
		TextComponent* m_TargetTextComponent{ nullptr };
	};
}