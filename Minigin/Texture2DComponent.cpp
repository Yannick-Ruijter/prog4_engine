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

SDL_Texture* dae::Texture2DComponent::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2DComponent::Texture2DComponent(GameObject* parent, const std::string &fullPath)
    :Component(parent)
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
        surface );

    SDL_DestroySurface(surface);

    if (!m_Texture)
    {
        throw std::runtime_error(
            std::string("Failed to create texture from surface: ") + SDL_GetError()
        );
    }
}

dae::Texture2DComponent::Texture2DComponent(GameObject* parent, SDL_Texture* texture)	
    :Component(parent), m_Texture{texture}
{
	assert(m_Texture != nullptr);
}

