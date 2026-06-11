#pragma once

#include "Component.hpp"
#include "GameStructs.hpp"
#include "Observer.hpp"
#include <memory>
#include <vector>

namespace dae {
class GameObject;
class Scene;
class LevelGrid;
class Font;
class LivesDisplay;
class TextDisplay;
enum class BurgerLayerType;
class GameManager : public Component, public Observer {
public:
  GameManager(GameObject &owner, LevelInfo const &levelInfo, Scene *scene);

  void SetupPlayers(GameObject *level);
  void RegisterPlayer(GameObject *player);
  void CreateBurger(glm::vec2 const &pos, BurgerLayerType type,
                    LevelGrid *levelGrid);
  void LoadBurgers(LevelGrid *levelGrid);
  void SaveBurgers();
  void AddPlayersToScene();
  int GetScore() const;
  GameObject *GetRandomPlayer() const;
  std::vector<GameObject *> const &GetPlayers() const;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<std::unique_ptr<GameObject>> m_TempPlayers{};
  std::vector<GameObject *> m_Players{};
  std::vector<GameObject *> m_Burgers{};
  Scene *m_Scene{nullptr};
  uint32_t m_CharactersDead{};
  std::shared_ptr<Font> m_TextFont;
  LevelInfo m_LevelInfo;
  TextDisplay *m_ScoreText{nullptr};
  int m_FinishedBurgerCount{};
  int m_TotalBurgers{};
  int m_NrOfLevels{3};
  int m_NrOfFallingBurgers{};
  bool m_PlayersDead{};
  bool m_ShouldRestart{false};

  void HandleEntityDeath(GameObject *object);
  void HandleBurgerFinished();
  void HandleScoreChange();
  void TryLoadingNextScene();

  // Inherited via Observer
};
} // namespace dae
