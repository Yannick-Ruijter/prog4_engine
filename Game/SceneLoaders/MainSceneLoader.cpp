#include "MainSceneLoader.hpp"

#include "Button.hpp"
#include "CustomCommands.hpp"
#include "FPSDisplay.hpp"
#include "GameSceneLoader.hpp"
#include "InputManager.hpp"
#include "LoadSceneLoader.hpp"
#include "ObjectRenderer.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "TextDisplay.hpp"
#include <GameOverSceneLoader.hpp>

using namespace dae;
// add a component that unloads all the bindings when scene destroyed
Scene *dae::MainSceneLoader::LoadScene() {
  auto scene = dae::SceneManager::GetInstance().CreateScene();
  auto &inputManager = dae::InputManager::GetInstance();
  auto go = std::make_unique<dae::GameObject>();
  auto fontMain =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
  auto fontSmall =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 24);

  // background stuff
  {
    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(292, 0);
    go->AddComponent<dae::TextDisplay>("Burger Time", fontMain,
                                       SDL_Color{100, 130, 200, 255});
    scene->Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->AddComponent<dae::TextDisplay>("60.0", fontSmall,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::FpsDisplay>();
    scene->Add(std::move(go));
  }

  dae::Button *initialButton;
  // create/link buttons
  {
    dae::Button *singlePlayerButton;
    dae::Button *coopButton;
    dae::Button *pvpButton;
    dae::Button *quitButton;

    SDL_Color idleColor{255, 255, 255, 255};
    SDL_Color focusedColor{255, 255, 0, 255};

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(50, 150);
    go->AddComponent<dae::TextDisplay>("Single Player", fontMain,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::Button>(
        [&]() {
          dae::SceneManager::GetInstance().LoadScene<dae::LoadSceneLoader>(
              LevelInfo{.gameMode = GameMode::SinglePlayer});
        },
        focusedColor, idleColor);
    singlePlayerButton = go->GetComponent<dae::Button>();
    scene->Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(50, 200);
    go->AddComponent<dae::TextDisplay>("Coop", fontMain,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::Button>(
        [&]() {
          dae::SceneManager::GetInstance().LoadScene<dae::LoadSceneLoader>(
              LevelInfo{.gameMode = GameMode::Coop});
        },
        focusedColor, idleColor);
    coopButton = go->GetComponent<dae::Button>();
    scene->Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(50, 250);
    go->AddComponent<dae::TextDisplay>("Pvp", fontMain,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::Button>(
        [&]() {
          dae::SceneManager::GetInstance().LoadScene<dae::LoadSceneLoader>(
              LevelInfo{.gameMode = GameMode::Pvp});
        },
        focusedColor, idleColor);
    pvpButton = go->GetComponent<dae::Button>();
    scene->Add(std::move(go));

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(50, 300);
    go->AddComponent<dae::TextDisplay>("Quit", fontMain,
                                       SDL_Color{255, 0, 0, 255});
    go->AddComponent<dae::Button>(
        [&]() {
          SDL_Event quit_event;
          quit_event.type = SDL_EVENT_QUIT;
          SDL_PushEvent(&quit_event);
        },
        focusedColor, idleColor);
    quitButton = go->GetComponent<dae::Button>();
    scene->Add(std::move(go));

    initialButton = singlePlayerButton;
    initialButton->GainFocus();

    singlePlayerButton->AddNeighbor(Direction::Up, quitButton);
    singlePlayerButton->AddNeighbor(Direction::Down, coopButton);

    coopButton->AddNeighbor(Direction::Up, singlePlayerButton);
    coopButton->AddNeighbor(Direction::Down, pvpButton);

    pvpButton->AddNeighbor(Direction::Up, coopButton);
    pvpButton->AddNeighbor(Direction::Down, quitButton);

    quitButton->AddNeighbor(Direction::Up, pvpButton);
    quitButton->AddNeighbor(Direction::Down, singlePlayerButton);
  }

  {
    auto controllerInput0 = inputManager.GetControllerInput(0);
    auto controllerInput1 = inputManager.GetControllerInput(1);
    auto keyboardInput = inputManager.GetKeyboardInput();

    dae::NavigateButtonCommand::SetInitialButton(initialButton);

    std::vector<Binding *> bindings{};
    bindings.emplace_back(controllerInput0->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Down),
        InputKeybinds::DPAD_DOWN, InputState::JustPressed));

    bindings.emplace_back(controllerInput0->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Up),
        InputKeybinds::DPAD_UP, InputState::JustPressed));

    bindings.emplace_back(controllerInput0->AddBinding(
        std::make_unique<dae::PressButtonCommand>(),
        InputKeybinds::BUTTON_SOUTH, InputState::JustPressed));

    bindings.emplace_back(controllerInput1->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Down),
        InputKeybinds::DPAD_DOWN, InputState::JustPressed));

    bindings.emplace_back(controllerInput1->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Up),
        InputKeybinds::DPAD_UP, InputState::JustPressed));

    bindings.emplace_back(controllerInput1->AddBinding(
        std::make_unique<dae::PressButtonCommand>(),
        InputKeybinds::BUTTON_SOUTH, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Up),
        InputKeybinds::UP, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Down),
        InputKeybinds::DOWN, InputState::JustPressed));

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
