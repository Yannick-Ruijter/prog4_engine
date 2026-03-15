#pragma once

#include <vector>
namespace dae {
	class Observer;
	enum class Event;
	class Subject
	{
	public:
		virtual void AddObserver(Observer* observer);
		virtual void RemoveObserver(Observer* observer);

	protected:
		virtual void NotifyObservers(Event const& event);
		std::vector<Observer*> m_Observers{};
	};
}