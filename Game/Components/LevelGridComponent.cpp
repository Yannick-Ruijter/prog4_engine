#include "LevelGridComponent.hpp"
#include "Renderer.hpp"
#include <SDL3/SDL.h>
#include <execution>

using namespace dae;

LevelGridComponent::LevelGridComponent(
    GameObject &owner, glm::ivec2 const &gridNumbers, glm::ivec2 const &windowDimensions)
    : Component(owner), m_GridNumbers{gridNumbers}
{
    m_GridDimensions.x = static_cast<float>(windowDimensions.x) / m_GridNumbers.x;
    m_GridDimensions.y = static_cast<float>(windowDimensions.y) / m_GridNumbers.y;
}

void dae::LevelGridComponent::Render() const
{
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255);
    for (int i = 0; i < m_GridNumbers.x * m_GridNumbers.y; ++i)
    {

        SDL_FRect currentRect{
            .x = (i % m_GridNumbers.x) * m_GridDimensions.x,
            .y = (i / m_GridNumbers.x) * m_GridDimensions.y,
            .w = m_GridDimensions.x,
            .h = m_GridDimensions.y};

        SDL_RenderRect(Renderer::GetInstance().GetSDLRenderer(), &currentRect);
    }
}
