#include "GameOverSceneLoader.hpp"

#include <Binding.hpp>
#include <Button.hpp>
#include <ControllerInput.hpp>
#include <CustomCommands.hpp>
#include <Font.hpp>
#include <GameObject.hpp>
#include <GameSceneLoader.hpp>
#include <HighScoresDisplay.hpp>
#include <InputInfo.hpp>
#include <InputManager.hpp>
#include <KeyboardInput.hpp>
#include <MainSceneLoader.hpp>
#include <ObjectRenderer.hpp>
#include <PlayerInput.hpp>
#include <ResourceManager.hpp>
#include <Scene.hpp>
#include <SceneManager.hpp>
#include <TextDisplay.hpp>
#include <Texture2DDisplay.hpp>
#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace dae;

Scene *dae::GameOverSceneLoader::LoadScene(LevelInfo levelInfo) {
  auto scene = dae::SceneManager::GetInstance().CreateScene();
  auto &inputManager = dae::InputManager::GetInstance();
  auto go = std::make_unique<dae::GameObject>();
  auto fontMain =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 36);
  auto fontSmall =
      dae::ResourceManager::GetInstance().LoadFont("PublicPixel.ttf", 24);

  // Input bindings
  {
    std::vector<ControllerInput *> inputs{
        inputManager.GetControllerInput(0),
        inputManager.GetControllerInput(1),
        inputManager.GetControllerInput(2),
        inputManager.GetControllerInput(3),
    };
    auto keyboardInput = inputManager.GetKeyboardInput();

    std::vector<Binding *> bindings{};
    for (auto input : inputs) {
      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::NavigateButtonCommand>(Direction::Down),
          InputKeybinds::DPAD_DOWN, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::NavigateButtonCommand>(Direction::Up),
          InputKeybinds::DPAD_UP, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::PressButtonCommand>(),
          InputKeybinds::BUTTON_SOUTH, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::ScrollButtonInitials>(Direction::Down),
          InputKeybinds::DPAD_DOWN, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::ScrollButtonInitials>(Direction::Up),
          InputKeybinds::DPAD_UP, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::ScrollButtonInitials>(Direction::Left),
          InputKeybinds::DPAD_LEFT, InputState::JustPressed));

      bindings.emplace_back(input->AddBinding(
          std::make_unique<dae::ScrollButtonInitials>(Direction::Right),
          InputKeybinds::DPAD_RIGHT, InputState::JustPressed));
    }

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Up),
        InputKeybinds::UP, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::NavigateButtonCommand>(Direction::Down),
        InputKeybinds::DOWN, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::ScrollButtonInitials>(Direction::Down),
        InputKeybinds::DOWN, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::ScrollButtonInitials>(Direction::Up),
        InputKeybinds::UP, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::ScrollButtonInitials>(Direction::Left),
        InputKeybinds::LEFT, InputState::JustPressed));

    bindings.emplace_back(keyboardInput->AddBinding(
        std::make_unique<dae::ScrollButtonInitials>(Direction::Right),
        InputKeybinds::RIGHT, InputState::JustPressed));

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

  go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(300.f, 20.f);
  go->AddComponent<dae::TextDisplay>("Game Over", fontMain,
                                     SDL_Color{255, 0, 0, 255});
  scene->Add(std::move(go));

  go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(250.f, 200.f);
  go->AddComponent<dae::TextDisplay>("Score Reached: ", fontMain,
                                     SDL_Color{50, 50, 255, 255});

  scene->Add(std::move(go));

  go = std::make_unique<GameObject>();
  go->AddComponent<ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(310.f, 250.f);
  size_t width = 8;
  // clang-format off
  auto text{
      std::string(
          width - std::min(width, std::to_string(levelInfo.currentScore).length()), '0') +
      std::to_string(levelInfo.currentScore) };
  // clang-format on
  go->AddComponent<TextDisplay>(text, fontSmall, SDL_Color{0, 255, 0, 255});

  scene->Add(std::move(go));

  go = std::make_unique<GameObject>();
  go->AddComponent<HighScoresDisplay>("Data/highscores.csv", fontSmall);
  auto highScores = go->GetComponent<HighScoresDisplay>();
  go->GetComponent<Transform>()->SetLocalPosition(300.f, 480.f);
  scene->Add(std::move(go));
  // save score button
  auto tempButton = std::make_unique<dae::GameObject>();
  auto initialsObject = tempButton.get();

  go = std::make_unique<dae::GameObject>();
  auto self = go.get();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(320.f, 350.f);
  go->AddComponent<dae::TextDisplay>("Save Score", fontSmall,
                                     SDL_Color{255, 255, 255, 255});
  go->AddComponent<dae::Button>(
      [levelInfo, initialsObject, self]() {
        auto initialsButton = initialsObject->GetComponent<Button>();
        auto selfButton = self->GetComponent<Button>();
        initialsButton->GainFocus();
        selfButton->LoseFocus();
        ButtonCommand::SetInitialButton(initialsButton);
      },
      SDL_Color{255, 255, 0, 255}, SDL_Color{100, 100, 100, 255});
  dae::Button *saveScoreButton = go->GetComponent<dae::Button>();
  saveScoreButton->GainFocus();
  scene->Add(std::move(go));

  tempButton->AddComponent<dae::ObjectRenderer>();
  tempButton->GetComponent<dae::Transform>()->SetLocalPosition(320.f, 400.f);
  tempButton->AddComponent<dae::TextDisplay>("Initials: ", fontSmall,
                                             SDL_Color{255, 255, 255, 255});
  tempButton->AddComponent<dae::Button>(
      [highScores, saveScoreButton, levelInfo, initialsObject]() {
        ButtonCommand::SetInitialButton(saveScoreButton);
        highScores->AddToHighscores(ScrollButtonInitials::Text,
                                    levelInfo.currentScore);
        saveScoreButton->GainFocus();
        initialsObject->GetComponent<Button>()->LoseFocus();
      },
      SDL_Color{255, 255, 0, 255}, SDL_Color{100, 100, 100, 255});
  auto initialsButton = tempButton->GetComponent<Button>();
  scene->Add(std::move(tempButton));
  // buttons for navigation game

  go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(320.f, 450.f);
  go->AddComponent<dae::TextDisplay>("Main Menu", fontSmall,
                                     SDL_Color{100, 100, 100, 255});
  go->AddComponent<dae::Button>(
      [levelInfo]() {
        SceneManager::GetInstance().LoadScene<MainSceneLoader>();
      },
      SDL_Color{255, 255, 0, 255}, SDL_Color{100, 100, 100, 255});
  dae::Button *mainMenuButton = go->GetComponent<dae::Button>();
  scene->Add(std::move(go));

  saveScoreButton->AddNeighbor(Direction::Up, mainMenuButton);
  saveScoreButton->AddNeighbor(Direction::Down, mainMenuButton);

  mainMenuButton->AddNeighbor(Direction::Up, saveScoreButton);
  mainMenuButton->AddNeighbor(Direction::Down, saveScoreButton);

  dae::ScrollButtonInitials::Text = "aa";
  dae::ScrollButtonInitials::CharIndex = 0;
  dae::NavigateButtonCommand::SetInitialButton(saveScoreButton);
  dae::ScrollButtonInitials::SetNameSelectionButton(initialsButton);
  return scene;
}

std::vector<std::pair<std::string, int>>
dae::GameOverSceneLoader::ReadScores() {
  std::ifstream stream{"Data/highscores.csv"};
  std::string line;
  std::vector<std::pair<std::string, int>> scores;
  while (std::getline(stream, line)) {
    const auto comma = line.find(',');
    if (comma == std::string::npos)
      continue;

    std::string initials = line.substr(0, comma);
    int score = std::stoi(line.substr(comma + 1));
    scores.emplace_back(std::move(initials), score);
  }
  return scores;
}

void dae::GameOverSceneLoader::ShowScores(Scene *scene,
                                          std::shared_ptr<Font> scoreFont) {

  auto go = std::make_unique<dae::GameObject>();
  go->AddComponent<dae::ObjectRenderer>();
  go->GetComponent<dae::Transform>()->SetLocalPosition(300.f, 480.f);
  go->AddComponent<dae::TextDisplay>("Highscores:", scoreFont,
                                     SDL_Color{255, 50, 100, 255});
  scene->Add(std::move(go));

  auto scores = ReadScores();
  std::sort(scores.begin(), scores.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });
  if (scores.size() > 10)
    scores.resize(10);

  constexpr float scoreStartY = 550.f;
  constexpr float scoreSpacing = 40.f;
  for (size_t i = 0; i < scores.size(); ++i) {
    const auto &[initials, score] = scores[i];
    std::string entry =
        initials + ": " +
        std::string(8 - std::min<size_t>(8, std::to_string(score).length()),
                    '0') +
        std::to_string(score);

    go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::ObjectRenderer>();
    go->GetComponent<dae::Transform>()->SetLocalPosition(
        300.f, scoreStartY + static_cast<float>(i) * scoreSpacing);
    go->AddComponent<dae::TextDisplay>(entry, scoreFont,
                                       SDL_Color{255, 50, 100, 255});
    scene->Add(std::move(go));
  }
}
