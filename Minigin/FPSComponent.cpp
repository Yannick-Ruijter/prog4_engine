#include "FPSComponent.h"
#include <memory>
#include "GameObject.h"
#include "TextComponent.h"
#include <format>

dae::FpsComponent::FpsComponent(GameObject& owner)
	:Component(owner) {}

void dae::FpsComponent::Update(float deltaTime)
{
	float currentFPS{ 1 / deltaTime };
	if (currentFPS - m_LastFps > 0.1f || currentFPS - m_LastFps < -0.1f)
	{
		m_LastFps = currentFPS;
		TextComponent* textComponent = GetOwner()->GetComponent<TextComponent>();
		textComponent->SetText(std::format("{:.1f} FPS", m_LastFps));
	}
}

float dae::FpsComponent::GetFps() const
{
	return m_LastFps;
}
