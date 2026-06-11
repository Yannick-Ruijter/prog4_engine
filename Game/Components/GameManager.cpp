
#include "Entity.hpp"
#include "GameManager.hpp"
#include "GameObject.hpp"
#include "GameSceneLoader.hpp"
#include "InputManager.hpp"
#include "LevelGrid.hpp"
#include "LivesDisplay.hpp"
#include "MainSceneLoader.hpp"
#include "ObjectRenderer.hpp"
#include "PepperDisplay.hpp"
#include "PlayerInput.hpp"
#include "PlayerPepperController.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "SpriteAnimation.hpp"
#include "Subject.hpp"
#include "TextDisplay.hpp"
#include "Texture2DDisplay.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"
#include <AIEnemyController.hpp>
#include <Font.hpp>
#include <ResourceManager.hpp>
#include <ServiceProvider.hpp>
#include <algorithm>
#include <random>
using namespace dae;

dae::GameManager::GameManager(GameObject &owner, LevelInfo const &levelInfo,
                              Scene *scene, LevelGrid *levelGrid)
    : Component(owner), m_Scene{scene}, m_LevelInfo{levelInfo},
      m_Level{levelGrid} {
  m_TextFont =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
}

void dae::GameManager::SetupPlayers() {

  auto CreatePlayerInfoDisplays = [&](GameObject *character, glm::vec2 pos) {
    std::string characterString{};
    auto entityComp{character->GetComponent<Entity>()};
    auto characterType{entityComp->GetCharacterType()};
    if (characterType == Character::MrPepper) {
      characterString = "PepperGuy";
    } else {
      characterString = "SaltWoman";
    }

    auto go = std::make_unique<GameObject>();
    go->GetComponent<Transform>()->SetLocalPosition(pos);
    go->AddComponent<LivesDisplay>(
        "Data/Characters/" + characterString + "_Head.png",
        m_LevelInfo.playerInfos.at(characterType).lives);
    auto livesDisplay{go->GetComponent<LivesDisplay>()};
    entityComp->GetDeathEvent()->AddObserver(livesDisplay);
    m_Scene->Add(std::move(go));

    go = std::make_unique<GameObject>();
    go->GetComponent<Transform>()->SetLocalPosition(pos + glm::vec2{80.f, 0.f});
    go->AddComponent<PepperDisplay>(
        "Data/Characters/" + characterString + "_Pepper.png",
        m_LevelInfo.playerInfos.at(characterType).nrPepper);
    auto pepperDisplay{go->GetComponent<PepperDisplay>()};
    entityComp->GetAttackEvent()->AddObserver(pepperDisplay);
    m_Scene->Add(std::move(go));
  };

  auto CreatePlayer = [&](Character character,
                          std::vector<PlayerInput *> const &inputs,
                          glm::vec2 infoPos) {
    if (!m_LevelInfo.playerInfos.contains(character))
      m_LevelInfo.playerInfos[character] = {};
    // not spawning if it's dead
    // this can only trigger in multiplayer
    if (m_LevelInfo.playerInfos.at(character).lives < 0)
      return;
    glm::ivec2 playerDimensions{32, 32};
    std::string spriteSheet{};
    if (character == Character::MrPepper)
      spriteSheet = "Data/Characters/PepperGuy_SpriteSheet.png";
    else
      spriteSheet = "Data/Characters/SaltWoman_SpriteSheet.png";
    auto go = std::make_unique<GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::Texture2DDisplay>(spriteSheet, playerDimensions.x,
                                            playerDimensions.y);
    go->AddComponent<dae::RectCollider>(Rect{glm::vec2{}, playerDimensions},
                                        LAYER_PEPPERGUY, LAYER_ENEMY);
    go->GetComponent<dae::Transform>()->SetLocalPosition(
        glm::vec3{150, 214, 0});

    go->AddComponent<dae::SpriteAnimation>(
        "Data/Characters/PepperGuy_AnimationData.json", spriteSheet);
    go->AddComponent<dae::Entity>(
        std::make_unique<dae::PlayerPepperController>(
            inputs, go.get(), m_LevelInfo.playerInfos.at(character).nrPepper),
        m_Level, character);
    auto controller = go->GetComponent<dae::Entity>()->GetInput();
    go->GetComponent<dae::RectCollider>()->GetSubject()->AddObserver(
        controller);
    go->GetComponent<Entity>()->GetDeathEvent()->AddObserver(this);

    CreatePlayerInfoDisplays(go.get(), infoPos);
    m_Players.push_back(go.get());
    m_TempPlayers.push_back(std::move(go));
  };

  auto &inputManager = InputManager::GetInstance();

  switch (m_LevelInfo.gameMode) {
  case GameMode::Pvp: {
    std::vector<PlayerInput *> inputs0{inputManager.GetControllerInput(1),
                                       inputManager.GetKeyboardInput()};
    CreatePlayer(Character::MrPepper, inputs0, glm::vec2{550.f, 5.f});
    break;
  }
  case GameMode::SinglePlayer: {
    std::vector<PlayerInput *> inputs0{inputManager.GetControllerInput(0),
                                       inputManager.GetKeyboardInput()};
    CreatePlayer(Character::MrPepper, inputs0, glm::vec2{550.f, 5.f});
    break;
  }
  case GameMode::Coop: {
    std::vector<PlayerInput *> inputs0{inputManager.GetControllerInput(1),
                                       inputManager.GetKeyboardInput()};
    std::vector<PlayerInput *> inputs1{inputManager.GetControllerInput(0)};
    CreatePlayer(Character::MrPepper, inputs0, glm::vec2{550.f, 5.f});
    CreatePlayer(Character::MrsSalt, inputs1, glm::vec2{730.f, 5.f});
    break;
  }
  }

  auto go = std::make_unique<GameObject>();
  go->AddComponent<ObjectRenderer>();
  size_t width = 8;
  // clang-format off
  auto text{
      std::string(
          width - std::min(width, std::to_string(m_LevelInfo.currentScore).length()), '0') +
      std::to_string(m_LevelInfo.currentScore)};
  // clang-format on
  go->GetComponent<Transform>()->SetLocalPosition(200.f, 10.f);
  go->AddComponent<TextDisplay>(text, m_TextFont);
  m_ScoreText = go->GetComponent<TextDisplay>();

  m_Scene->Add(std::move(go));
}

void dae::GameManager::RegisterPlayer(GameObject *player) {
  m_Players.emplace_back(player);
}

void dae::GameManager::CreateBurger(glm::vec2 const &pos,
                                    BurgerLayerType type) {
  auto go = std::make_unique<GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(pos);
  go->AddComponent<dae::BurgerLayer>(type, m_Level);
  auto layer{go->GetComponent<BurgerLayer>()};
  layer->BurgerFellEvent()->AddObserver(this);
  if (layer->Type() == BurgerLayerType::TopPaddy) {
    layer->BurgerFinishedEvent()->AddObserver(this);
    m_TotalBurgers++;
  }
  auto dimensions = go->GetComponent<dae::BurgerLayer>()->GetDimensions();
  go->AddComponent<dae::RectCollider>(Rect{pos, dimensions}, LAYER_BURGER,
                                      LAYER_NONE);
  m_Burgers.push_back(go.get());
  m_Scene->Add(std::move(go));
}

void dae::GameManager::LoadBurgers() {
  for (auto const &burgerInfo : m_LevelInfo.burgerInfos) {
    CreateBurger(burgerInfo.pos, burgerInfo.type);
  }
}

void dae::GameManager::SaveBurgers() {
  m_LevelInfo.burgerInfos.clear();
  for (auto burger : m_Burgers) {
    auto transform = burger->GetComponent<Transform>();
    auto type = burger->GetComponent<BurgerLayer>()->Type();
    m_LevelInfo.burgerInfos.emplace_back(transform->GetWorldPosition(), type);
  }
}

void dae::GameManager::AddPlayersToScene() {
  for (auto &player : m_TempPlayers) {
    m_Scene->Add(std::move(player));
  }
  m_TempPlayers.clear();
}

int dae::GameManager::GetScore() const { return m_LevelInfo.currentScore; }

LevelGrid *dae::GameManager::GetLevel() const { return m_Level; }

LevelInfo const &dae::GameManager::GetLevelInfo() const { return m_LevelInfo; }

// get a random player from the list (created for ai controller enemies
GameObject *dae::GameManager::GetRandomPlayer() const {
  if (m_Players.size() == 0)
    return nullptr;
  return m_Players[std::uniform_int_distribution<size_t>(
      0, m_Players.size() - 1)(ServiceProvider::GetRandomProvider().GetRng())];
}

std::vector<GameObject *> const &dae::GameManager::GetPlayers() const {
  return m_Players;
}

void dae::GameManager::Notify(EventId eventId, GameObject *source) {
  if (eventId == "OnEntityDeath"_h) {
    auto entityType{source->GetComponent<Entity>()->GetCharacterType()};
    // clang-format off
    if(entityType == Character::HotDogGuy) m_LevelInfo.currentScore += 100;
    if(entityType == Character::PickleGuy) m_LevelInfo.currentScore += 200;
    if(entityType == Character::EggGuy) m_LevelInfo.currentScore += 300;
    HandleScoreChange();
    // clang-format on
    HandleEntityDeath(source);
  }

  if (eventId == "BurgerFinished"_h) {
    HandleBurgerFinished();
  }
  if (eventId == "StartedFalling"_h) {
    m_NrOfFallingBurgers++;
  }
  if (eventId == "BurgerFell"_h) {
    m_LevelInfo.currentScore += 50;
    HandleScoreChange();
    m_NrOfFallingBurgers--;
    TryLoadingNextScene();
  }
}

void dae::GameManager::HandleEntityDeath(GameObject *object) {
  if (auto entity = object->GetComponent<Entity>(); entity) {
    if (auto entityType = entity->GetCharacterType();
        m_LevelInfo.playerInfos.contains(entityType)) {
      auto controller{
          dynamic_cast<PlayerPepperController *>(entity->GetInput())};
      // it can not be null
      assert(controller != nullptr);
      m_LevelInfo.playerInfos.at(entityType).nrPepper =
          controller->GetPepperCount();
      std::erase(m_Players, object);
      m_CharactersDead++;
      // check if all players died this stage
      if (m_CharactersDead == m_LevelInfo.playerInfos.size()) {
        // check if any of the players still has a life left
        m_PlayersDead = true;
        for (auto const &characterInfo : m_LevelInfo.playerInfos) {
          if (characterInfo.second.lives > 0)
            m_PlayersDead = false;
        }
        m_LevelInfo.playerInfos.at(entityType).lives--;
        m_ShouldRestart = true;

        TryLoadingNextScene();
      }
    }
  }
}

void dae::GameManager::HandleBurgerFinished() {
  m_FinishedBurgerCount++;
  if (m_FinishedBurgerCount == m_TotalBurgers) {
    m_LevelInfo.burgerInfos.clear();
    m_LevelInfo.level++;
    if (m_LevelInfo.level >= m_NrOfLevels)
      m_LevelInfo.level = 0;

    SceneManager::GetInstance().LoadScene<GameSceneLoader>(m_LevelInfo);
  }
}

void dae::GameManager::HandleScoreChange() {
  size_t width = 8;
  // clang-format off
    auto text{
        std::string(
            width - std::min(width, std::to_string(m_LevelInfo.currentScore).length()), '0') +
        std::to_string(m_LevelInfo.currentScore) };
    m_ScoreText->SetText(text);
}

void dae::GameManager::TryLoadingNextScene()
{
    if (m_NrOfFallingBurgers == 0 && m_ShouldRestart) {
        if (m_PlayersDead) {
            // temporarily load main scene if they al have no lives left
            SceneManager::GetInstance().LoadScene<MainSceneLoader>();
        }
        else {
            SaveBurgers();
            SceneManager::GetInstance().LoadScene<GameSceneLoader>(m_LevelInfo);
        }
    }
}
