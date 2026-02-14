#include "Texture2DComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"

void dae::RenderComponent::AddToGameObject(GameObject* parent)
{
	parent->m_Components.renderComponent = std::make_unique<RenderComponent>();
}

dae::RenderComponent* dae::RenderComponent::GetFromObject(GameObject* object)
{
	return object->m_Components.renderComponent.get();
}

void dae::RenderComponent::Render(Texture2DComponent* texture, TransformComponent const * transform) const
{
	Renderer::GetInstance().RenderTexture(*texture, transform->GetPosition().x, transform->GetPosition().y);
}

