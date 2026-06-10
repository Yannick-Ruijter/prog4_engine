#pragma once
#include <unordered_map>
namespace dae {

enum class GameMode {
  SinglePlayer,
  Coop,
  Pvp,
};

enum class Character { MrPepper, MrsSalt, HotDogGuy };

struct PlayerInfo {
  int lives = 3;
  int nrPepper = 5;
};
// by default has the values for loading the game for the first time
struct LevelInfo {
  GameMode gameMode = GameMode::SinglePlayer;
  int level = 0;
  int currentScore = 0;
  std::unordered_map<Character, PlayerInfo> playerInfos = {};
};

} // namespace dae
