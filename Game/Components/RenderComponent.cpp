#include "Texture2DComponent.hpp"
#include "RenderComponent.hpp"
#include "GameObject.hpp"
#include "TransformComponent.hpp"
#include "Renderer.hpp"

dae::RenderComponent::RenderComponent(GameObject& owner) :Component(owner), m_TransformComponent{ owner.GetComponent<TransformComponent>()}, m_Texture2DComponent{nullptr} {}

void dae::RenderComponent::Update()
{
	if (m_Texture2DComponent == nullptr) m_Texture2DComponent = GetOwner()->GetComponent<Texture2DComponent>();
}

void dae::RenderComponent::Render() const
{
	if (m_Texture2DComponent == nullptr) return;

	Renderer::GetInstance().RenderTexture(*m_Texture2DComponent, m_TransformComponent->GetWorldPosition().x, m_TransformComponent->GetWorldPosition().y);
}

