#pragma once

namespace dae
{
    class GameObject;
    class Scene;
    enum class GameMode
    {
        SinglePlayer,
        Coop,
        Pvp,
    };

    // by default has the values for loading the game for the first time
    struct LevelInfo
    {
        GameMode gameMode = GameMode::SinglePlayer;
        int level = 0;
        int lifeCount = 3;
        int currentScore = 0;
    };

    class GameSceneLoader
    {
      public:
        static Scene *LoadScene(LevelInfo levelInfo = {});
    };
} // namespace dae
