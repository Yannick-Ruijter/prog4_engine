#pragma once

#include <vector>
namespace dae {
	class Observer;
	class GameObject;
	enum class Event;
	class Subject
	{
	public:
		Subject() = default;
		~Subject() = default;
		virtual void AddObserver(Observer* observer);
		virtual void RemoveObserver(Observer* observer);
		virtual void NotifyObservers(Event const& event, GameObject* source);

	protected:
		std::vector<Observer*> m_Observers{};
	};
}