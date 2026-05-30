#pragma once
#include "glm/glm.hpp"
#include <vector>

namespace dae {
    class Scene;
    class GameObject;
    class LevelGridComponent;
    enum class GameMode {
        SinglePlayer,
        Coop,
        Pvp,
    };

    // by default has the values for loading the game for the first time
    struct LevelInfo {
        GameMode gameMode = GameMode::SinglePlayer;
        int level = 0;
        int lifeCount = 3;
        int currentScore = 0;
    };

    class GameSceneLoader {
      public:
        static Scene *LoadScene(LevelInfo levelInfo = {});

      private:
        static void LoadSpriteMap(
            Scene *scene, glm::ivec2 const &tileSize, std::vector<GameObject *> const &players,
            LevelGridComponent *levelGrid);
    };
} // namespace dae
