#pragma once

#include <vector>
namespace dae {
	class Observer;
	class GameObject;
	enum class Event;
	class Subject
	{
	public:
		virtual void AddObserver(Observer* observer);
		virtual void RemoveObserver(Observer* observer);

	protected:
		virtual void NotifyObservers(Event const& event, GameObject* source);
		std::vector<Observer*> m_Observers{};
	};
}