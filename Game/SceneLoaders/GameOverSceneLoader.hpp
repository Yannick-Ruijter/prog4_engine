#pragma once
#include "GameStructs.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <vector>
namespace dae {
class Scene;
class Font;
class GameOverSceneLoader {
public:
  static Scene *LoadScene(LevelInfo levelInfo = {});

private:
  static std::vector<std::pair<std::string, int>> ReadScores();
  static void ShowScores(Scene *scene, std::shared_ptr<Font> scoreFont);
};
} // namespace dae
