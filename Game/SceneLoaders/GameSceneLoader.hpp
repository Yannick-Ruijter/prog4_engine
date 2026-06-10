#pragma once
#include "GameStructs.hpp"
#include "glm/glm.hpp"
#include <vector>

namespace dae {
class Scene;
class GameObject;
class LevelGrid;
class GameSceneLoader {
public:
  static Scene *LoadScene(LevelInfo levelInfo = {});

private:
  static void LoadSpriteMap(Scene *scene, glm::ivec2 const &tileSize,
                            std::vector<GameObject *> const &players,
                            LevelGrid *levelGrid);
};
} // namespace dae
