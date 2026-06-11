#include "GameSceneLoader.hpp"

#include "Binding.hpp"
#include "CustomCommands.hpp"
#include "Entity.hpp"
#include "FPSDisplay.hpp"
#include "GameManager.hpp"
#include "GameObject.hpp"
#include "Health.hpp"
#include "InputManager.hpp"
#include "LevelGrid.hpp"
#include "LivesDisplay.hpp"
#include "ObjectRenderer.hpp"
#include "PlayerEnemyController.hpp"
#include "PlayerPepperController.hpp"
#include "RectCollider.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "Score.hpp"
#include "ScoreDisplay.hpp"
#include "SpriteAnimation.hpp"
#include "Subject.hpp"
#include "TextDisplay.hpp"
#include "burgerLayer.hpp"
#include <AIEnemyController.hpp>
#include <EnemySpawner.hpp>
#include <SpriteAnimation.hpp>
#include <Texture2DDisplay.hpp>

#include <fstream>
#include <sstream>

using namespace dae;

Scene *dae::GameSceneLoader::LoadScene(LevelInfo levelInfo) {
  auto scene = dae::SceneManager::GetInstance().CreateScene();
  auto go = std::make_unique<dae::GameObject>();
  // auto fontMain =
  //     dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);

  auto fontSmall =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 24);

  glm::ivec2 const tileSize{64, 64};
  LevelGrid *levelGrid{nullptr};
  // background stuff
  {
    std::unordered_map<char, std::string> charToTexture{};
    charToTexture['0'] = "Data/Tiles/Platform_Ladder_Bottom.png";
    charToTexture['1'] = "Data/Tiles/Platform_Burger_Left.png";
    charToTexture['2'] = "Data/Tiles/Platform_Burger_Right.png";
    charToTexture['3'] = "Data/Tiles/Platform_Ladder_Top.png";
    charToTexture['4'] = "Data/Tiles/Platform_Ladder_Small_Left.png";
    charToTexture['5'] = "Data/Tiles/Platform_Ladder_Small_Right.png";
    charToTexture['6'] = "Data/Tiles/Ladder.png";
    charToTexture['7'] = "Data/Tiles/Ladder_Small_Left.png";
    charToTexture['8'] = "Data/Tiles/Ladder_Small_Right.png";
    charToTexture['9'] = "Data/Tiles/Burger_Bowl_Left.png";
    charToTexture['a'] = "Data/Tiles/Burger_Bowl_Middle.png";
    charToTexture['b'] = "Data/Tiles/Burger_Bowl_Double.png";
    charToTexture['c'] = "Data/Tiles/Burger_Bowl_Right.png";

    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::LevelGrid>(
        tileSize,
        "Data/Levels/Level" + std::to_string(levelInfo.level) + ".csv",
        charToTexture);
    levelGrid = go->GetComponent<LevelGrid>();
    scene->Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::TextDisplay>("60.0", fontSmall,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::FpsDisplay>();
    scene->Add(std::move(go));
  }

  go = std::make_unique<GameObject>();
  go->AddComponent<GameManager>(levelInfo, scene, levelGrid);
  auto manager = go->GetComponent<GameManager>();
  manager->SetupPlayers();
  std::vector<glm::vec2> spawnPoints{};
  LoadSpriteMap(tileSize, manager, levelInfo, spawnPoints);
  manager->AddPlayersToScene();
  scene->Add(std::move(go));

  go = std::make_unique<GameObject>();
  go->AddComponent<EnemySpawner>(manager, 10.f, spawnPoints, scene);

  scene->Add(std::move(go));
  {
    std::vector<Binding *> bindings{};

    auto keyBoardInput{InputManager::GetInstance().GetKeyboardInput()};
    bindings.push_back(keyBoardInput->AddBinding(
        std::make_unique<AdvanceSceneCommand>(manager), InputKeybinds::F1,
        InputState::JustPressed));

    scene->AddExitFunction([bindings]() {
      auto &inputManager{InputManager::GetInstance()};
      auto keyBoardInput{inputManager.GetKeyboardInput()};
      std::vector<PlayerInput *> controllerInputs{
          inputManager.GetControllerInput(0),
          inputManager.GetControllerInput(1),
          inputManager.GetControllerInput(2),
          inputManager.GetControllerInput(3)};

      for (auto const &binding : bindings) {
        keyBoardInput->UnBind(binding);
        for (auto const &controllerInput : controllerInputs)
          controllerInput->UnBind(binding);
      }
    });
  }
  return scene;
}

void dae::GameSceneLoader::LoadSpriteMap(glm::ivec2 const &tileSize,
                                         GameManager *manager,
                                         LevelInfo levelInfo,
                                         auto &spawnPoints) {
  bool loadBurgers{levelInfo.burgerInfos.size() == 0};
  if (!loadBurgers)
    manager->LoadBurgers();
  std::string const &filePath{"Data/Levels/Level" +
                              std::to_string(levelInfo.level) + "_Burgers.csv"};
  std::ifstream stream{filePath};
  std::string line;
  glm::ivec2 gridCoord{};
  std::unordered_map<char, BurgerLayerType> layers{
      {'0', BurgerLayerType::TopPaddy},
      {'1', BurgerLayerType::Tomato},
      {'2', BurgerLayerType::Salad},
      {'3', BurgerLayerType::BottomPaddy},
  };
  char spawnPointChar{'z'};
  auto players = manager->GetPlayers();
  std::unordered_map<char, GameObject *> charToPlayers;
  for (uint32_t i = 0; i < players.size(); i++) {
    charToPlayers[char('a' + i)] = players[i];
  }
  while (std::getline(stream, line)) {
    std::stringstream ss{line};
    std::string cell;

    // goes over every character between ','
    while (std::getline(ss, cell, ',')) {
      char currentChar{cell[0]};
      if (currentChar == spawnPointChar) {
        glm::vec2 pos{gridCoord * tileSize + tileSize / 4};
        pos.y += tileSize.y / 8;
        spawnPoints.push_back(pos);
      }
      if (charToPlayers.contains(currentChar)) {
        glm::vec2 pos{gridCoord * tileSize + tileSize / 4};
        pos.y += tileSize.y / 8;
        auto playerTransform =
            charToPlayers.at(currentChar)->GetComponent<Transform>();
        playerTransform->SetLocalPosition(pos);
      }
      if (loadBurgers && layers.contains(currentChar)) {
        glm::vec2 pos{gridCoord * tileSize};
        // to place them at exactly the right spot since they don't have the
        // same size as the tiles
        pos.x += tileSize.x / 3.5f;
        pos.y += tileSize.y / 1.9f;
        manager->CreateBurger(pos, layers.at(currentChar));
      }
      gridCoord.x++;
    }
    gridCoord.x = 0;
    gridCoord.y++;
  }
}
