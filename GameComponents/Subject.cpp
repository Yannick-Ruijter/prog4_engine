
#include "Subject.h"
#include "Observer.h"
#include "Event.h"

using namespace dae;

void Subject::AddObserver(Observer* observer)
{
	m_Observers.emplace_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::find(begin(m_Observers), end(m_Observers), observer));
}

void Subject::NotifyObservers(Event const& event, GameObject* source)
{
	for (auto const& observer : m_Observers)
	{
		observer->Notify(event, source);
	}
}