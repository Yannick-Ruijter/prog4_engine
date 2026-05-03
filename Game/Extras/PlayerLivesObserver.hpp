#pragma once

#include "Observer.hpp"


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
		void Notify(unsigned int eventId, GameObject* source) override;
	private:
		TextComponent* m_TargetTextComponent{ nullptr };
	};
}