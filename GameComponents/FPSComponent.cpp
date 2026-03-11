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
	if (currentFPS - m_LastFps > 0.1f || currentFPS - m_LastFps < -0.1f || m_LastFps == 0.0f)
	{
		if(m_TextComponent == nullptr) m_TextComponent = GetOwner()->GetComponent<TextComponent>();
		if (m_TextComponent == nullptr) return;
		m_LastFps = currentFPS;
		std::string fps = std::to_string(m_LastFps);
		fps.resize(4);
		fps += " FPS";
		m_TextComponent->SetText(fps);
	}
}

float dae::FpsComponent::GetFps() const
{
	return m_LastFps;
}
