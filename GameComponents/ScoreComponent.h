#pragma once
#include "Component.h"
#include <memory>
#include "Observer.h"
namespace dae
{
	class Subject;
	class ScoreComponent : public Component, public Observer
	{
	public:
		ScoreComponent(GameObject& owner, std::unique_ptr<Subject>&& scoreChangedEvent = nullptr);
		void AddScore(int score);
		int GetScore() const;

		void Notify(Event event, GameObject* source);
		Subject* GetSubject() const;
	private:
		std::unique_ptr<Subject> m_ScoreChangedEvent{ nullptr };
		int m_CurrentScore{ 0 };
	};
}