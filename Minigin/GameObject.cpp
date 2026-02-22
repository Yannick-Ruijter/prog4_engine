#include <string>
#include "GameObject.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


void dae::GameObject::Update(float deltaTime)
{

	for (auto const& component : m_MyComponents)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto const& component : m_MyComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepCoordinates)
{
	if (parent == m_Parent) return;

	assert(parent != this && "You can not become your own parent!");

	assert(std::find(begin(m_Children), end(m_Children), this) == end(m_Children) && "You can not become the child of your own child!");

	if (m_Parent != nullptr)
	{
		m_Parent->m_Children.erase(
			std::find(begin(m_Parent->m_Children), end(m_Parent->m_Children), this),
			m_Parent->m_Children.end()
		);
	}

	m_Parent = parent;
	m_Parent->m_Children.emplace_back(this);

	if (!keepCoordinates)
	{
		TransformComponent* transform{ GetComponent<TransformComponent>() };
		TransformComponent* otherTransform{ m_Parent->GetComponent<TransformComponent>() };
		assert(otherTransform != nullptr && "Parent gameobject does not have a transform");
		if (transform == nullptr) AddComponent<TransformComponent>(otherTransform->GetPosition());
		else transform->SetPosition(otherTransform->GetPosition());
	}
}


