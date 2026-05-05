#pragma once

#include <vector>
namespace dae
{
    class Observer;
    class GameObject;
    enum class Event;
    class Subject
    {
      public:
        Subject() = default;
        virtual ~Subject() = default;
        virtual Subject *AddObserver(Observer *observer);
        virtual Subject *RemoveObserver(Observer *observer);
        virtual void NotifyObservers(unsigned int eventId, GameObject *source);

      protected:
        std::vector<Observer *> m_Observers{};
    };
} // namespace dae
