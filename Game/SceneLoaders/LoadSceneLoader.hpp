#pragma once
#include "GameStructs.hpp"

namespace dae {
class Scene;
class LoadSceneLoader {
public:
  static Scene *LoadScene(LevelInfo levelInfo = {});
};
} // namespace dae
