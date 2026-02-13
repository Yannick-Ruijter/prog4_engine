#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include "TextComponent.h"
#include "Font.h"

void dae::GameObject::Update(float deltaTime)
{
	if (m_TextComponent.get())	m_Texture = m_TextComponent->GetTexture();
	if (m_FpsComponent.get()) m_FpsComponent->Update(deltaTime);
}

void dae::GameObject::Render() const
{
	if (m_RenderComponent.get() == nullptr) return;
	if (m_Texture.get()) m_RenderComponent->Render(m_Texture.get(), m_Transform);
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	std::string temp = filename;
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddRenderComponent()
{
	m_RenderComponent = std::make_unique<RenderComponent>(this);
}

void dae::GameObject::AddFpsComponent()
{
	m_FpsComponent = std::make_unique<FpsComponent>();
}

void dae::GameObject::AddTextComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
{
	assert(m_RenderComponent.get() != nullptr && "There is no Render component on this gameobject");
	m_TextComponent = std::make_unique<TextComponent>(text, font, color);
	m_Texture = m_TextComponent->GetTexture();
}

void dae::GameObject::SetRenderText(std::string const& text)
{
	assert(m_TextComponent.get() != nullptr && "There is no Text component on this gameobject");
	m_TextComponent->SetText(text);
}

void dae::GameObject::SetRenderTextColor(const SDL_Color& color)
{
	assert(m_TextComponent.get() != nullptr && "There is no Text component on this gameobject");
	m_TextComponent->SetColor(color);
}

bool dae::GameObject::HasFpsComponent() const
{
	return m_FpsComponent.get() != nullptr;
}

float dae::GameObject::GetFps()
{
	assert(m_FpsComponent.get() != nullptr);
	return m_FpsComponent->GetFps();
}
