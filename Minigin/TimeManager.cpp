#include "TimeManager.h"

using namespace dae;

float TimeManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

void TimeManager::Update()
{
	auto currentTime{ std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() };
	m_DeltaTime = (currentTime - m_PreviousTime) / 1000000.f;
	m_PreviousTime = currentTime;
}