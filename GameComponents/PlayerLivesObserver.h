#pragma once

#include "Observer.h"


enum class Event;
namespace dae
{
	class GameObject;
	class PlayerLivesObserver : public Observer
	{
	public:
		PlayerLivesObserver() = default;
		~PlayerLivesObserver() override = default;
		void Notify(Event event, GameObject* object) override;
	};
}