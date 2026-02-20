#include "Texture2DComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject& owner) :Component(owner), m_Texture2DComponent{ nullptr }, m_TransformComponent{ nullptr } {}

void dae::RenderComponent::Update(float)
{
	if (m_Texture2DComponent == nullptr) m_Texture2DComponent = GetOwner()->GetComponent<Texture2DComponent>();
	if (m_TransformComponent == nullptr) m_TransformComponent = GetOwner()->GetComponent<TransformComponent>();
}

void dae::RenderComponent::Render() const
{
	if (m_Texture2DComponent == nullptr) return;
	if (m_TransformComponent == nullptr) return;

	Renderer::GetInstance().RenderTexture(*m_Texture2DComponent, m_TransformComponent->GetPosition().x, m_TransformComponent->GetPosition().y);
}

