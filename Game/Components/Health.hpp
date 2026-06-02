#pragma once
#include "Component.hpp"
#include <memory>
namespace dae
{
	class GameObject;
	class Subject;
	class Health : public Component
	{
	public:
		Health(GameObject& owner, std::unique_ptr<Subject>&& liveLostEvent = nullptr, int numberOfLives = 3);
		~Health() override;
		void Damage();
		int GetNumberOfLives() const;
		Subject* GetSubject() const;

	private:
		int m_NumberOfLives{ 0 };
		std::unique_ptr<Subject> m_LiveLostEvent{ nullptr };
	};
}