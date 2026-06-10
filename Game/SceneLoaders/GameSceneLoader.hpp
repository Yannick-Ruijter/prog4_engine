#pragma once
#include "GameStructs.hpp"
#include "glm/glm.hpp"
#include <vector>

namespace dae {
class Scene;
class GameObject;
class LevelGrid;
class GameManager;
class GameSceneLoader {
public:
  static Scene *LoadScene(LevelInfo levelInfo = {});

private:
  static void LoadSpriteMap(glm::ivec2 const &tileSize, LevelGrid *levelGrid,
                            GameManager *manager, LevelInfo levelInfo);
};
} // namespace dae
