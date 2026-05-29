#pragma once
#include "SceneLoader.hpp"

namespace dae
{
    class GameObject;
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

    class GameSceneLoader : public SceneLoader
    {
      public:
        GameSceneLoader(LevelInfo levelInfo = {});
        virtual ~GameSceneLoader() override;
        virtual Scene *AcquireScene() const override;

      private:
        Scene *m_Scene;
    };
} // namespace dae
