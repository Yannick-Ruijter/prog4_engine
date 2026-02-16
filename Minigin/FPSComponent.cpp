#include "FPSComponent.h"
#include <memory>
#include "GameObject.h"
#include "TextComponent.h"

void dae::FpsComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.fpsComponent = std::make_unique<FpsComponent>(parent);
}

dae::FpsComponent* dae::FpsComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.fpsComponent.get();
}

dae::FpsComponent::FpsComponent(GameObject* parent)
	:Component(parent) {}

void dae::FpsComponent::Update(float deltaTime)
{
	m_LastFps = 1 / deltaTime;
	/*TextComponent* textComponent = m_Parent->GetComponent<TextComponent>();
	textComponent->SetText(std::to_string(m_LastFps));*/
}

float dae::FpsComponent::GetFps() const
{
	return m_LastFps;
}
