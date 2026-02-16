#include "Texture2DComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"

void dae::RenderComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.renderComponent = std::make_unique<RenderComponent>(parent);
}

dae::RenderComponent* dae::RenderComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.renderComponent.get();
}

dae::RenderComponent::RenderComponent(GameObject* parent) :Component(parent) {}

void dae::RenderComponent::Render(Texture2DComponent* texture, TransformComponent const * transform) const
{
	Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
	/*Texture2DComponent* texturee = m_Parent->GetComponent<Texture2DComponent>();
	TransformComponent* transformm = m_Parent->GetComponent<TransformComponent>();
	Renderer::GetInstance().RenderTexture(*texturee, transformm->GetPosition().x, transformm->GetPosition().y);*/
}

