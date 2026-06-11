#pragma once

#include <Component.hpp>
#include <GameStructs.hpp>
#include <Observer.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace dae {
class GameManager;
class Scene;
class EnemySpawner : public Component, public Observer {
public:
  EnemySpawner(GameObject &owner, GameManager *manager, float spawnFrequency,
               std::vector<glm::vec2> const &spawnPositions, Scene *scene);
  virtual void Update() override;
  virtual void Notify(EventId event, GameObject *source) override;

  void SpawnEnemy(glm::vec2 const &pos, Character character,
                  bool playerControlled = false);

private:
  std::vector<glm::vec2> m_SpawnPositions{};
  GameManager *m_Manager{nullptr};
  Scene *m_Scene{nullptr};
  float m_SpawnSpeed{};
  float m_SpawnTimer{};
  uint32_t m_SpawnIndex{};
  int m_PlayerEnemyCount{};
  int m_EggCount{};

  glm::vec2 GetNextSpawnPos();
  Character GetNextCharacterType();
  bool ShouldBePlayerControlled();
  std::string_view charToString(Character character);
};
} // namespace dae
