#include "Texture2D.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject* parent)
	:m_Parent{parent}
{
	assert(parent != nullptr);
}

void dae::RenderComponent::Render(Texture2D* texture, Transform const& transform) const
{
	Renderer::GetInstance().RenderTexture(*texture, transform.GetPosition().x, transform.GetPosition().y);
}

