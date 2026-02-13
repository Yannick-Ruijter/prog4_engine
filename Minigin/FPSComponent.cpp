#include "FPSComponent.h"
#include <chrono>

void FpsComponent::Update(float deltaTime)
{
	m_LastFps = 1 / deltaTime;
}

float FpsComponent::GetFps()
{
	return m_LastFps;
}
