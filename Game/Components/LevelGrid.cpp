#include "LevelGrid.hpp"
#include "Renderer.hpp"
#include <execution>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace dae;

LevelGrid::LevelGrid(GameObject &owner, glm::ivec2 const &gridSize,
                     std::string const &levelFile,
                     std::unordered_map<char, std::string> textures)
    : Component(owner), m_GridSize{gridSize} {
  std::ifstream stream{levelFile};
  std::string line;
  m_TexturesLoaded[' '] = nullptr;
  while (std::getline(stream, line)) {
    m_Grid.emplace_back();
    std::stringstream ss{line};
    std::string cell;

    while (std::getline(ss, cell, ',')) {
      char currentChar{cell[0]};
      m_Grid.back().emplace_back(currentChar);
      if (textures.contains(currentChar) &&
          !m_TexturesLoaded.contains(currentChar)) {
        SDL_Surface *surface = SDL_LoadPNG(textures[currentChar].c_str());
        surface = SDL_ScaleSurface(surface, gridSize.x, gridSize.y,
                                   SDL_ScaleMode::SDL_SCALEMODE_PIXELART);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(
            Renderer::GetInstance().GetSDLRenderer(), surface);
        SDL_DestroySurface(surface);
        m_TexturesLoaded[currentChar] = texture;
      }
    }
  }
}

void dae::LevelGrid::Render() const {
  for (size_t y = 0; y < m_Grid.size(); ++y) {
    for (size_t x = 0; x < m_Grid[y].size(); ++x) {
      char cell = m_Grid[y][x];
      if (!m_TexturesLoaded.contains(cell) ||
          m_TexturesLoaded.at(cell) == nullptr)
        continue;

      SDL_FRect dst{static_cast<float>(x * m_GridSize.x),
                    static_cast<float>(y * m_GridSize.y),
                    static_cast<float>(m_GridSize.x),
                    static_cast<float>(m_GridSize.y)};
      SDL_RenderTexture(Renderer::GetInstance().GetSDLRenderer(),
                        m_TexturesLoaded.at(cell), nullptr, &dst);
    }
  }
}

bool dae::LevelGrid::IsOnPlatform(glm::vec2 const &topleft, glm::vec2 size) {
  // check if all 2 corners are on platform grids
  // first 6 tiles are all platforms

  char topLeftTile{GetTile(topleft)};
  if (topLeftTile > '5' || topLeftTile < '0')
    return false;

  char bottomRightTile{GetTile(topleft + size)};
  if (bottomRightTile > '5' || bottomRightTile < '0')
    return false;

  // check if y is close enough to platform
  int distanceToPlatform{static_cast<int>(topleft.y + size.y) % m_GridSize.y};
  if (distanceToPlatform < (m_GridSize.y - 15) && distanceToPlatform > 3)
    return false;

  return true;
}

bool dae::LevelGrid::IsOnLadder(glm::vec2 const &topleft, glm::vec2 size) {
  char bottomRightTile{GetTile(topleft + size)};
  char bottomLeftTile{GetTile(glm::vec2(topleft.x, topleft.y + size.y))};

  // Both on center ladder tiles
  auto isCenterLadder = [](char t) { return t == '3' || t == '6'; };
  if (isCenterLadder(bottomLeftTile) && isCenterLadder(bottomRightTile))
    return true;

  // Left on left-edge, right on right-edge
  auto isLeftEdge = [](char t) { return t == '4' || t == '7'; };
  auto isRightEdge = [](char t) { return t == '5' || t == '8'; };
  if (isLeftEdge(bottomLeftTile) && isRightEdge(bottomRightTile))
    return true;

  return false;
}

bool dae::LevelGrid::IsOnBurgerBowl(glm::vec2 const &topleft, glm::vec2 size) {
  // check tile for bottomleft and bottomright to be in burgerbowl
  char bottomRightTile{GetTile(topleft + size)};
  if (!(bottomRightTile >= '9' && bottomRightTile <= 'c'))
    return false;
  char bottomLeftTile{GetTile(glm::vec2(topleft.x, topleft.y + size.y))};
  if (!(bottomLeftTile >= '9' && bottomLeftTile <= 'c'))
    return false;

  return true;
}

float dae::LevelGrid::RoundToPlatformHeight(float yPos) {
  float platformOffset = 55.f;
  float relative = yPos - platformOffset;
  float nearestRow = std::round(relative / m_GridSize.y);
  return nearestRow * m_GridSize.y + platformOffset;
}

int dae::LevelGrid::GetGridSize() const { return m_GridSize.x; }

char dae::LevelGrid::GetTile(glm::vec2 const &pos) {
  uint32_t row = static_cast<uint32_t>(pos.y / m_GridSize.y);
  uint32_t column = static_cast<uint32_t>(pos.x / m_GridSize.x);
  if (row < m_Grid.size() && column < m_Grid.front().size())
    return m_Grid[row][column];
  return ' ';
}

bool dae::LevelGrid::IsPlatform(char tile) {
  return tile >= '0' && tile <= '5';
}

bool dae::LevelGrid::IsLadder(char tile) { return tile >= '3' && tile <= '8'; }
