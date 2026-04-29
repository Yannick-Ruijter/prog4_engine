#pragma once

namespace dae
{
	class GameObject;
	enum class Event;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(unsigned int eventId, GameObject* source) = 0;
	};
}