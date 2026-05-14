#include "LevelGridComponent.hpp"
#include "Renderer.hpp"
#include <execution>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace dae;

LevelGridComponent::LevelGridComponent(
    GameObject &owner, glm::ivec2 const &gridSize, std::string const &levelFile,
    std::unordered_map<char, std::string> textures)
    : Component(owner), m_GridSize{gridSize}
{
    std::ifstream stream{levelFile};
    std::string line;
    m_TexturesLoaded[' '] = nullptr;
    while (std::getline(stream, line))
    {
        m_Grid.emplace_back();
        std::stringstream ss{line};
        std::string cell;

        while (std::getline(ss, cell, ','))
        {
            char currentChar{cell[0]};
            m_Grid.back().emplace_back(currentChar);
            if (textures.contains(currentChar) && !m_TexturesLoaded.contains(currentChar))
            {
                SDL_Surface *surface = SDL_LoadPNG(textures[currentChar].c_str());
                surface = SDL_ScaleSurface(surface, gridSize.x, gridSize.y, SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
                SDL_DestroySurface(surface);
                m_TexturesLoaded[currentChar] = texture;
            }
        }
    }
}

void dae::LevelGridComponent::Render() const
{
    for (size_t y = 0; y < m_Grid.size(); ++y)
    {
        for (size_t x = 0; x < m_Grid[y].size(); ++x)
        {
            char cell = m_Grid[y][x];
            if (!m_TexturesLoaded.contains(cell) || m_TexturesLoaded.at(cell) == nullptr)
                continue;

            SDL_FRect dst{
                static_cast<float>(x * m_GridSize.x), static_cast<float>(y * m_GridSize.y),
                static_cast<float>(m_GridSize.x), static_cast<float>(m_GridSize.y)};
            SDL_RenderTexture(Renderer::GetInstance().GetSDLRenderer(), m_TexturesLoaded.at(cell), nullptr, &dst);
        }
    }
}
