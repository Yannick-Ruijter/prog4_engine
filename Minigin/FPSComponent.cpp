#include "FPSComponent.h"
#include <memory>
#include "GameObject.h"


void dae::FpsComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.fpsComponent = std::make_unique<FpsComponent>();
}

dae::FpsComponent* dae::FpsComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.fpsComponent.get();
}

void dae::FpsComponent::Update(float deltaTime)
{
	m_LastFps = 1 / deltaTime;
}

float dae::FpsComponent::GetFps() const
{
	return m_LastFps;
}
