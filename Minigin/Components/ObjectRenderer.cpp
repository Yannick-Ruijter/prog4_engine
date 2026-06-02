#include "GameObject.hpp"
#include "ObjectRenderer.hpp"
#include "Renderer.hpp"
#include "Texture2DDisplay.hpp"
#include "Transform.hpp"

dae::ObjectRenderer::ObjectRenderer(GameObject &owner)
    : Component(owner), m_Transform{owner.GetComponent<Transform>()}, m_Texture2DDisplay{nullptr} {
}

void dae::ObjectRenderer::Update() {
    if (m_Texture2DDisplay == nullptr)
        m_Texture2DDisplay = GetOwner()->GetComponent<Texture2DDisplay>();
}

void dae::ObjectRenderer::Render() const {
    if (m_Texture2DDisplay == nullptr)
        return;

    Renderer::GetInstance().RenderTexture(
        *m_Texture2DDisplay, m_Transform->GetWorldPosition().x, m_Transform->GetWorldPosition().y);
}
