#include "Texture2DComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject& owner) :Component(owner) {}

void dae::RenderComponent::Render(/*Texture2DComponent* texture, TransformComponent const * transform*/) const
{
	//Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
	Texture2DComponent* texture = GetOwner()->GetComponent<Texture2DComponent>();
	if(texture == nullptr)
		texture = GetOwner()->GetComponent<TextComponent>()->GetTexture().get();
	TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
	Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
}

