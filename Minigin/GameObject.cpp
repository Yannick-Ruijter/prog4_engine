#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


void dae::GameObject::Update(float deltaTime)
{
	if (m_Components.textComponent.get())	m_Components.texture2DComponent = m_Components.textComponent->GetTexture();
	if (m_Components.fpsComponent.get()) m_Components.fpsComponent->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	if (m_Components.renderComponent.get() == nullptr) return;
	if (m_Components.texture2DComponent.get()) m_Components.renderComponent->Render(m_Components.texture2DComponent.get(), m_Components.transform.get());
}

//void dae::GameObject::AddComponent(Component* component)
//{
//	m_MyComponents.push_back(std::make_unique<Componnent>(component));
//}
