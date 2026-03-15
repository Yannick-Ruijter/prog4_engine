#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject& owner, int numberOfLives = 3);
		void Damage();
		int GetNumberOfLives() const;

	private:
		int m_NumberOfLives{ 0 };

	};
}