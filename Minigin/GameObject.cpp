#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"


void dae::GameObject::Update(float deltaTime)
{
	for (auto const& child : m_Children)
	{
		child->Update(deltaTime);
	}

	for (auto const& component : m_MyComponents)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto const& child : m_Children)
	{
		child->Render();
	}

	for (auto const& component : m_MyComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepCoordinates)
{
	if (IsChild(parent) || parent == this || m_Parent == parent) return;

	if (parent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (keepCoordinates)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}

	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

glm::vec3 dae::GameObject::GetWorldPosition() const
{
	return m_Transform->GetWorldPosition();	
}

dae::GameObject::GameObject(GameObject* parent, bool keepCoordinates)
{
	SetParent(parent, keepCoordinates);
}

bool dae::GameObject::IsChild(GameObject* object) const
{
	return std::find_if(begin(m_Children), end(m_Children), [object](const auto& ptr) {return ptr.get() == object; }) != end(m_Children);
}

void dae::GameObject::RemoveChild(GameObject* object)
{
	m_Children.erase(std::find_if(begin(m_Children), end(m_Children), [object](const auto& ptr) {return ptr.get() == object; }));
}

void dae::GameObject::AddChild(GameObject* object)
{
	m_Children.emplace_back(object);
}

void dae::GameObject::SetLocalPosition(glm::vec3 const& pos)
{
	m_Transform->SetLocalPosition(pos);
}

void dae::GameObject::SetPositionDirty()
{
	m_UpdatePosition = true;
}
