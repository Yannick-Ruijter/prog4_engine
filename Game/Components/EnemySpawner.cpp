#include "EnemySpawner.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"
#include "RectCollider.hpp"
#include <AIEnemyController.hpp>
#include <GameObject.hpp>
#include <InputManager.hpp>
#include <ObjectRenderer.hpp>
#include <PlayerEnemyController.hpp>
#include <Scene.hpp>
#include <SpriteAnimation.hpp>
#include <Texture2DDisplay.hpp>
#include <TimeManager.hpp>
#include <sdbm_hash.hpp>

using namespace dae;

dae::EnemySpawner::EnemySpawner(GameObject &owner, GameManager *manager,
                                float spawnFrequency,
                                std::vector<glm::vec2> const &spawnPositions,
                                Scene *scene)
    : Component(owner), m_SpawnPositions{spawnPositions}, m_Manager{manager},
      m_Scene{scene}, m_SpawnSpeed{spawnFrequency} {}

void dae::EnemySpawner::Update() {
  m_SpawnTimer += TimeManager::GetInstance().GetDeltaTime();

  if (m_SpawnTimer >= m_SpawnSpeed) {
    auto pos = GetNextSpawnPos();
    auto type = GetNextCharacterType();
    auto playerControlled = ShouldBePlayerControlled();
    SpawnEnemy(pos, type, playerControlled);
    m_SpawnTimer = 0.f;
  }
}

void dae::EnemySpawner::Notify(EventId event, GameObject *source) {
  if (event == "OnEntityDeath"_h) {
    if (auto entity = source->GetComponent<Entity>(); entity) {
      if (entity->GetCharacterType() == Character::EggGuy)
        --m_EggCount;
    }
  }
}

void dae::EnemySpawner::SpawnEnemy(glm::vec2 const &pos, Character character,
                                   bool playerControlled) {
  auto go = std::make_unique<GameObject>();

  std::unique_ptr<InputProvider> input;
  {
    if (playerControlled)
      input = std::make_unique<PlayerEnemyController>(
          std::vector<PlayerInput *>{
              InputManager::GetInstance().GetControllerInput(0)},
          go.get());
    else
      input = std::make_unique<AIEnemyController>(go.get(), m_Manager);
  }
  float moveSpeed{};
  if (character == Character::EggGuy) {
    m_EggCount++;
    moveSpeed = 80.f;
  }
  if (character == Character::PickleGuy)
    moveSpeed = 70.f;
  if (character == Character::HotDogGuy)
    moveSpeed = 60.f;
  go->AddComponent<ObjectRenderer>();

  std::string charString{charToString(character)};
  go->AddComponent<dae::Texture2DDisplay>(
      "Data/Characters/" + charString + "_SpriteSheet.png", 32, 32);
  go->AddComponent<dae::RectCollider>(Rect{glm::vec2{}, glm::vec2{32.f, 32.f}},
                                      LAYER_ENEMY, LAYER_BURGER | LAYER_PEPPER);
  go->GetComponent<Transform>()->SetLocalPosition(pos);
  go->AddComponent<dae::SpriteAnimation>(
      "Data/Characters/Enemy_AnimationData.json",
      "Data/Characters/" + charString + "_SpriteSheet.png");
  go->AddComponent<dae::Entity>(std::move(input), m_Manager->GetLevel(),
                                character, moveSpeed);
  auto entity = go->GetComponent<dae::Entity>();
  entity->GetDeathEvent()->AddObserver(this);
  auto controller = entity->GetInput();
  go->GetComponent<dae::RectCollider>()->GetSubject()->AddObserver(controller);

  m_Scene->Add(std::move(go));
}

glm::vec2 dae::EnemySpawner::GetNextSpawnPos() {
  m_SpawnIndex++;
  if (m_SpawnIndex >= m_SpawnPositions.size())
    m_SpawnIndex = 0;

  return m_SpawnPositions[m_SpawnIndex];
}

Character dae::EnemySpawner::GetNextCharacterType() {
  if (m_EggCount == 0)
    return Character::EggGuy;
  if (m_Manager->GetLevelInfo().level < 2)
    return Character::HotDogGuy;

  return rand() % 2 ? Character::HotDogGuy : Character::PickleGuy;
}

bool dae::EnemySpawner::ShouldBePlayerControlled() {
  return m_Manager->GetLevelInfo().gameMode == GameMode::Pvp &&
         m_PlayerEnemyCount == 0;
}

std::string_view dae::EnemySpawner::charToString(Character character) {
  switch (character) {
  case Character::HotDogGuy:
    return "HotDogGuy";
  case Character::EggGuy:
    return "EggGuy";
  case Character::PickleGuy:
    return "PickleGuy";
  default:
    return "";
  }
}
