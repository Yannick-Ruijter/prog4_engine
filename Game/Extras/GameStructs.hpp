#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
namespace dae {
enum class BurgerLayerType;
enum class GameMode {
  SinglePlayer,
  Coop,
  Pvp,
};

enum class Character { MrPepper, MrsSalt, HotDogGuy, EggGuy, PickleGuy };

struct PlayerInfo {
  int lives = 3;
  int nrPepper = 5;
};

struct BurgerInfo {
  glm::vec2 pos;
  BurgerLayerType type;
};
// by default has the values for loading the game for the first time
struct LevelInfo {
  GameMode gameMode = GameMode::SinglePlayer;
  int level = 0;
  int currentScore = 0;
  std::unordered_map<Character, PlayerInfo> playerInfos = {};
  std::vector<BurgerInfo> burgerInfos = {};
};

} // namespace dae
