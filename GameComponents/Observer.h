#pragma once

namespace dae
{
	class GameObject;
	enum class Event;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event, GameObject* source) = 0;
	};
}