#include "FPSComponent.h"
#include <memory>
#include "GameObject.h"
#include "TextComponent.h"
#include <format>

dae::FpsComponent::FpsComponent(GameObject* parent)
	:Component(parent) {}

void dae::FpsComponent::Update(float deltaTime)
{
	m_LastFps = 1 / deltaTime;
	TextComponent* textComponent = m_Parent->GetComponent<TextComponent>();
	textComponent->SetText(std::format("{:.1f} FPS", m_LastFps));
}

float dae::FpsComponent::GetFps() const
{
	return m_LastFps;
}
