#pragma once
#include "Component.hpp"
#include <memory>
#include "Observer.hpp"
namespace dae
{
	class Subject;
	class ScoreComponent : public Component, public Observer
	{
	public:
		ScoreComponent(GameObject& owner, std::unique_ptr<Subject>&& scoreChangedEvent = nullptr);
		void AddScore(int score);
		int GetScore() const;

		void Notify(unsigned int eventId, GameObject* source);
		Subject* GetSubject() const;
	private:
		std::unique_ptr<Subject> m_ScoreChangedEvent{ nullptr };
		int m_CurrentScore{ 0 };
	};
}