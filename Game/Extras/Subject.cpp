
#include "Subject.hpp"
#include "Observer.hpp"

using namespace dae;

Subject *Subject::AddObserver(Observer *observer)
{
    m_Observers.emplace_back(observer);
    return this;
}

Subject *Subject::RemoveObserver(Observer *observer)
{
    m_Observers.erase(std::find(begin(m_Observers), end(m_Observers), observer));
    return this;
}

void Subject::NotifyObservers(unsigned int eventId, GameObject *source)
{
    for (auto const &observer : m_Observers)
    {
        observer->Notify(eventId, source);
    }
}
