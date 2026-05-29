#pragma once

namespace dae
{
    typedef unsigned int EventId;
    class GameObject;
    enum class Event;
    class Observer
    {
      public:
        virtual ~Observer() = default;
        virtual void Notify(EventId eventId, GameObject *source) = 0;
    };
} // namespace dae
