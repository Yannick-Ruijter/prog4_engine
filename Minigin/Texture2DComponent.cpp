#include <SDL3/SDL.h>
#include "Texture2DComponent.h"
#include "Renderer.h"
#include <stdexcept>
#include "GameObject.h"
#include "ResourceManager.h"

dae::Texture2DComponent::~Texture2DComponent()
{
	SDL_DestroyTexture(m_Texture);
}

glm::vec2 dae::Texture2DComponent::GetSize() const
{
    float w{}, h{};
    SDL_GetTextureSize(m_Texture, &w, &h);
    return { w, h };
}

void dae::Texture2DComponent::AddToGameObject(GameObject* parent, const std::string& fullPath)
{
    parent->m_Components.texture2DComponent = ResourceManager::GetInstance().LoadTexture(fullPath);;
}

void dae::Texture2DComponent::AddToGameObject(GameObject* parent, SDL_Texture* texture)
{
    parent->m_Components.texture2DComponent = std::make_shared<Texture2DComponent>(texture);
}

dae::Texture2DComponent* dae::Texture2DComponent::GetFromObject(GameObject* object)
{
    return object->m_Components.texture2DComponent.get();
}

SDL_Texture* dae::Texture2DComponent::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2DComponent::Texture2DComponent(const std::string &fullPath)
{
    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if (!surface)
    {
        throw std::runtime_error(
            std::string("Failed to load PNG: ") + SDL_GetError()
        );
    }

    m_Texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        surface
    );

    SDL_DestroySurface(surface);

    if (!m_Texture)
    {
        throw std::runtime_error(
            std::string("Failed to create texture from surface: ") + SDL_GetError()
        );
    }
}

dae::Texture2DComponent::Texture2DComponent(SDL_Texture* texture)	: m_Texture{ texture } 
{
	assert(m_Texture != nullptr);
}

