#include "SoundEventQueue.hpp"

using namespace dae;

void dae::SoundEventQueue::Quit()
{
    m_IsSleeping = true;
    m_ConditionVariable.notify_all();
}

//yes im converting a lambda to a std::function
void dae::SoundEventQueue::AddToQueue(std::function<void()> execution)
{
    {
        std::lock_guard lock{ m_Mutex };
        m_Queue.push(std::move(execution));
    }

    m_ConditionVariable.notify_one();
}

void dae::SoundEventQueue::DoAllSounds() {
    std::unique_lock lock{ m_Mutex };
    m_ConditionVariable.wait(lock, [this] { return !m_Queue.empty() || m_IsSleeping; });

    while (!m_Queue.empty()) 
    {
        auto execution = std::move(m_Queue.front());
        m_Queue.pop();
        lock.unlock();
        execution();
        lock.lock();
    }
}