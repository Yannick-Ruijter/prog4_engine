#include "LoadSceneLoader.hpp"

#include <Binding.hpp>
#include <Button.hpp>
#include <ControllerInput.hpp>
#include <CustomCommands.hpp>
#include <Font.hpp>
#include <GameObject.hpp>
#include <GameSceneLoader.hpp>
#include <InputInfo.hpp>
#include <InputManager.hpp>
#include <KeyboardInput.hpp>
#include <ObjectRenderer.hpp>
#include <PlayerInput.hpp>
#include <ResourceManager.hpp>
#include <Scene.hpp>
#include <SceneManager.hpp>
#include <TextDisplay.hpp>
#include <Texture2DDisplay.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace dae;

Scene *dae::LoadSceneLoader::LoadScene(LevelInfo levelInfo) {
  auto scene = dae::SceneManager::GetInstance().CreateScene();
  auto &inputManager = dae::InputManager::GetInstance();
  auto go = std::make_unique<dae::GameObject>();
  auto fontMain =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
  auto fontSmall =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 24);
  const bool isSinglePlayer = levelInfo.gameMode == GameMode::SinglePlayer;
  constexpr float screenCenterX = 320.f; // adjust to your screen width / 2
  constexpr float screenCenterY = 400.f; // adjust to your screen height
  constexpr float playerSpread = 200.f;

  // Player label positions
  struct PlayerInfo {
    std::string name;
    SDL_Color color;
    float x;
  };

  std::vector<PlayerInfo> players;
  if (isSinglePlayer) {
    players.push_back(
        {"Player 1", SDL_Color{100, 130, 200, 255}, screenCenterX});
  } else {
    players.push_back({"Player 1", SDL_Color{100, 130, 200, 255},
                       screenCenterX - playerSpread});
    players.push_back({"Player 2", SDL_Color{255, 105, 180, 255},
                       screenCenterX + playerSpread});
  }

  for (auto const &player : players) {
    // "Player X" label
    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(player.x, 150.f);
    go->AddComponent<dae::TextDisplay>(player.name, fontMain, player.color);
    scene->Add(std::move(go));

    // "Ready" label beneath
    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(player.x, 200.f);
    go->AddComponent<dae::TextDisplay>("Ready", fontMain, player.color);
    scene->Add(std::move(go));
  }

  // Invisible button with no neighbors and empty onpress
  go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(screenCenterX - 100.f,
                                                       screenCenterY + 200.f);
  go->AddComponent<dae::TextDisplay>("Press attack to start", fontSmall,
                                     SDL_Color{255, 255, 255, 255});
  go->AddComponent<dae::Button>(
      [levelInfo]() {
        SceneManager::GetInstance().LoadScene<GameSceneLoader>(levelInfo);
      },
      SDL_Color{255, 255, 255, 255}, SDL_Color{255, 255, 255, 255});
  dae::Button *startButton = go->GetComponent<dae::Button>();
  startButton->GainFocus();
  scene->Add(std::move(go));

  // Input bindings
  {
    auto controllerInput0 = inputManager.GetControllerInput(0);
    auto controllerInput1 = inputManager.GetControllerInput(1);
    auto keyboardInput = inputManager.GetKeyboardInput();

    dae::NavigateButtonCommand::SetInitialButton(startButton);

    std::vector<Binding *> bindings{};

    bindings.emplace_back(controllerInput0->AddBinding(
        std::make_unique<dae::PressButtonCommand>(), InputKeybinds::BUTTON_WEST,
        InputState::JustPressed));

    bindings.emplace_back(controllerInput1->AddBinding(
        std::make_unique<dae::PressButtonCommand>(), InputKeybinds::BUTTON_WEST,
        InputState::JustPressed));

    bindings.emplace_back(
        keyboardInput->AddBinding(std::make_unique<dae::PressButtonCommand>(),
                                  InputKeybinds::S, InputState::JustPressed));

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
