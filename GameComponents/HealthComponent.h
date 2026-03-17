#pragma once
#include "Component.h"
#include <memory>
namespace dae
{
	class GameObject;
	class Subject;
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject& owner, std::unique_ptr<Subject>&& liveLostEvent = nullptr, int numberOfLives = 3);
		~HealthComponent() override;
		void Damage();
		int GetNumberOfLives() const;
		Subject* GetSubject() const;

	private:
		int m_NumberOfLives{ 0 };
		std::unique_ptr<Subject> m_LiveLostEvent{ nullptr };
	};
}