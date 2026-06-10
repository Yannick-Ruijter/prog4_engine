#pragma once

#include "Component.hpp"
#include "GameStructs.hpp"
#include "Observer.hpp"
#include <memory>
#include <vector>

namespace dae {
class GameObject;
class Scene;
class GameManager : public Component, public Observer {
public:
  GameManager(GameObject &owner, LevelInfo const &levelInfo);

  void SetupPlayers(GameObject *level);
  void RegisterPlayer(GameObject *player);
  void AddPlayersToScene(Scene *scene);
  int GetScore() const;
  std::vector<GameObject *> const &GetPlayers() const;
  void Notify(EventId eventId, GameObject *source) override;

private:
  std::vector<std::unique_ptr<GameObject>> m_TempPlayers{};
  std::vector<GameObject *> m_Players{};
  uint32_t m_CharactersDead{};
  LevelInfo m_LevelInfo;

  // Inherited via Observer
};
} // namespace dae
