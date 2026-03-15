#pragma once

#include "Subject.h"
#include "Observer.h"
#include "Event.h"
using namespace dae;

void Subject::AddObserver(Observer* observer)
{
	m_Observers.emplace_back(observer);
}

void Subject::RemoveObserver(Observer*)
{

}

void Subject::NotifyObservers(Event const&)
{
	//for (auto const& observer : m_Observers)
	//{
	//	//observer->Notify(Event::PlayerDied)
	//}
}