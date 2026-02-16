#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


void dae::GameObject::Update(float deltaTime)
{

	/*for (auto const& component : m_MyComponents)
	{
		component->Update(deltaTime);
	}*/
	if (m_Components.textComponent.get())	m_Components.texture2DComponent = m_Components.textComponent->GetTexture();
	if (m_Components.fpsComponent.get()) m_Components.fpsComponent->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	/*for (auto const& component : m_MyComponents)
	{
		component->Render();
	}*/
	if (m_Components.renderComponent.get() == nullptr) return;
	if (m_Components.texture2DComponent.get()) m_Components.renderComponent->Render(m_Components.texture2DComponent.get(), m_Components.transform.get());
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> component)
{
	m_MyComponents.push_back(std::move(component));
}

template <typename T>
T* dae::GameObject::GetComponent() const
{
	for (auto const& component : m_MyComponents)
	{
		T* component = dynamic_cast<T*>(component.get());
		if (component != nullptr) return component;
	}
	return nullptr;
}
