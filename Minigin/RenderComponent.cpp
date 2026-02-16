#include "Texture2DComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject* parent) :Component(parent) {}

void dae::RenderComponent::Render(/*Texture2DComponent* texture, TransformComponent const * transform*/) const
{
	//Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
	Texture2DComponent* texture = m_Parent->GetComponent<Texture2DComponent>();
	if(texture == nullptr)
		texture = m_Parent->GetComponent<TextComponent>()->GetTexture().get();
	TransformComponent* transform = m_Parent->GetComponent<TransformComponent>();
	Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
}

