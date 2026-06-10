#pragma once

#include "Component.hpp"
#include "GameStructs.hpp"
#include "Observer.hpp"
#include <memory>
#include <vector>

namespace dae {
class GameObject;
class Scene;
class LivesDisplay;
class GameManager : public Component, public Observer {
public:
  GameManager(GameObject &owner, LevelInfo const &levelInfo, Scene *scene);

  void SetupPlayers(GameObject *level);
  void RegisterPlayer(GameObject *player);
  void AddPlayersToScene();
  int GetScore() const;
  std::vector<GameObject *> const &GetPlayers() const;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<std::unique_ptr<GameObject>> m_TempPlayers{};
  std::vector<GameObject *> m_Players{};
  Scene *m_Scene{nullptr};
  uint32_t m_CharactersDead{};
  LevelInfo m_LevelInfo;

  // Inherited via Observer
};
} // namespace dae
