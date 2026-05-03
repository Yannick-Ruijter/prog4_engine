#pragma once

#include <functional>
#include <mutex>
#include <queue>

namespace dae {
	class SoundEventQueue
	{
	public:
		void Quit();
		void AddToQueue(std::function<void()> execution);
		void DoAllSounds();
	private:
		mutable std::mutex m_Mutex;
		std::atomic<bool> m_IsSleeping{ true };
		std::condition_variable m_ConditionVariable;
		std::queue<std::function<void()>> m_Queue{};
	};
}