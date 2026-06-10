
#include "Entity.hpp"
#include "GameManager.hpp"
#include "GameObject.hpp"
#include "GameSceneLoader.hpp"
#include "InputManager.hpp"
#include "LevelGrid.hpp"
#include "MainSceneLoader.hpp"
#include "ObjectRenderer.hpp"
#include "PlayerInput.hpp"
#include "PlayerPepperController.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "SpriteAnimation.hpp"
#include "Subject.hpp"
#include "Texture2DDisplay.hpp"
#include "sdbm_hash.hpp"
#include <algorithm>
using namespace dae;

dae::GameManager::GameManager(GameObject &owner, LevelInfo const &levelInfo)
    : Component(owner), m_LevelInfo{levelInfo} {}

void dae::GameManager::SetupPlayers(GameObject *level) {
  auto CreatePlayer = [&](Character character,
                          std::vector<PlayerInput *> const &inputs) {
    if (!m_LevelInfo.playerInfos.contains(character))
      m_LevelInfo.playerInfos[character] = {};
    // not spawning if it's dead
    // this can only trigger in multiplayer
    if (m_LevelInfo.playerInfos.at(character).lives == 0)
      return;

    glm::ivec2 playerDimensions{32, 32};
    auto go = std::make_unique<GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::Texture2DDisplay>(
        "Data/pepperguy.png", playerDimensions.x, playerDimensions.y);
    go->AddComponent<dae::RectCollider>(Rect{glm::vec2{}, playerDimensions},
                                        LAYER_PEPPERGUY, LAYER_ENEMY);
    go->GetComponent<dae::Transform>()->SetLocalPosition(
        glm::vec3{150, 214, 0});
    go->AddComponent<dae::SpriteAnimation>(
        "Data/Characters/PepperGuy_AnimationData.json",
        "Data/Characters/PepperGuy_SpriteSheet.png");
    go->AddComponent<dae::Entity>(
        std::make_unique<dae::PlayerPepperController>(
            inputs, go.get(), m_LevelInfo.playerInfos.at(character).nrPepper),
        level->GetComponent<dae::LevelGrid>(), character);
    auto controller = go->GetComponent<dae::Entity>()->GetInput();
    go->GetComponent<dae::RectCollider>()->GetSubject()->AddObserver(
        controller);
    go->GetComponent<Entity>()->GetDeathEvent()->AddObserver(this);
    m_Players.push_back(go.get());
    m_TempPlayers.push_back(std::move(go));
  };

  auto &inputManager = InputManager::GetInstance();

  switch (m_LevelInfo.gameMode) {
  case GameMode::Pvp:
  case GameMode::SinglePlayer: {
    std::vector<PlayerInput *> inputs0{inputManager.GetControllerInput(0),
                                       inputManager.GetKeyboardInput()};
    CreatePlayer(Character::MrPepper, inputs0);
    break;
  }
  case GameMode::Coop: {
    std::vector<PlayerInput *> inputs0{inputManager.GetControllerInput(1),
                                       inputManager.GetKeyboardInput()};
    std::vector<PlayerInput *> inputs1{inputManager.GetControllerInput(0)};
    CreatePlayer(Character::MrPepper, inputs0);
    CreatePlayer(Character::MrsSalt, inputs1);
    break;
  }
  }
}

void dae::GameManager::RegisterPlayer(GameObject *player) {
  m_Players.emplace_back(player);
}

void dae::GameManager::AddPlayersToScene(Scene *scene) {
  for (auto &player : m_TempPlayers) {
    scene->Add(std::move(player));
  }
  m_TempPlayers.clear();
}

int dae::GameManager::GetScore() const { return m_LevelInfo.currentScore; }

std::vector<GameObject *> const &dae::GameManager::GetPlayers() const {
  return m_Players;
}

void dae::GameManager::Notify(EventId eventId, GameObject *source) {
  if (eventId == "OnEntityDeath"_h) {
    if (auto entity = source->GetComponent<Entity>(); entity) {
      if (auto entityType = entity->GetCharacterType();
          m_LevelInfo.playerInfos.contains(entityType)) {
        m_LevelInfo.playerInfos.at(entityType).lives--;
        auto controller{
            dynamic_cast<PlayerPepperController *>(entity->GetInput())};
        // it can not be null
        assert(controller != nullptr);

        m_LevelInfo.playerInfos.at(entityType).nrPepper =
            controller->GetPepperCount();
        m_CharactersDead++;
        // check if all players died
        if (m_CharactersDead == m_LevelInfo.playerInfos.size()) {
          bool noLivesLeft{true};
          for (auto const &characterInfo : m_LevelInfo.playerInfos) {
            if (characterInfo.second.lives > 0)
              noLivesLeft = false;
          }

          if (noLivesLeft) {
            // temporarily load main scene if they al have no lives left
            SceneManager::GetInstance().LoadScene<MainSceneLoader>();
          } else {
            SceneManager::GetInstance().LoadScene<GameSceneLoader>(m_LevelInfo);
          }
        }
        return;
      }
    }
  }

  if (eventId == "OnBurgerFinished"_h) {
  }
}
