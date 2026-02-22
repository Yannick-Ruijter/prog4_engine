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


