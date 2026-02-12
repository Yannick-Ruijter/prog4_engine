#include "FPSComponent.h"
#include <chrono>
void FpsComponent::StartLoop()
{
	auto now = std::chrono::system_clock::now();
	m_StartLoopTime = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
}

void FpsComponent::EndLoop()
{
	auto now = std::chrono::system_clock::now();
	m_EndLoopTime = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
	m_LastFps = 1.f / (m_EndLoopTime - m_StartLoopTime);
}

float FpsComponent::GetFps()
{
	return m_LastFps;
}
