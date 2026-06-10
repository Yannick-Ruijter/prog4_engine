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
class LivesDisplay;
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
  std::vector<GameObject *> const &GetPlayers() const;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<std::unique_ptr<GameObject>> m_TempPlayers{};
  std::vector<GameObject *> m_Players{};
  std::vector<GameObject *> m_Burgers{};
  Scene *m_Scene{nullptr};
  uint32_t m_CharactersDead{};
  LevelInfo m_LevelInfo;
  int m_FinishedBurgerCount{};
  int m_TotalBurgers{};
  // Inherited via Observer
};
} // namespace dae
