#include "MainSceneLoader.hpp"

#include "ButtonComponent.hpp"
#include "CustomCommands.hpp"
#include "FPSComponent.hpp"
#include "GameSceneLoader.hpp"
#include "InputManager.hpp"
#include "RenderComponent.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "TextComponent.hpp"

using namespace dae;

Scene *dae::MainSceneLoader::LoadScene() {
    auto scene = dae::SceneManager::GetInstance().CreateScene();
    auto &inputManager = dae::InputManager::GetInstance();
    auto go = std::make_unique<dae::GameObject>();
    auto fontMain = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    // auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

    // background stuff
    {

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 0);
        go->AddComponent<dae::TextComponent>("Burger Time", fontMain, SDL_Color{255, 0, 0, 255});
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->AddComponent<dae::TextComponent>("60.0", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::FpsComponent>();
        scene->Add(std::move(go));
    }

    dae::ButtonComponent *initialButton;
    // create/link buttons
    {
        dae::ButtonComponent *singlePlayerButton;
        dae::ButtonComponent *coopButton;
        dae::ButtonComponent *pvpButton;
        dae::ButtonComponent *quitButton;

        SDL_Color idleColor{255, 255, 255, 255};
        SDL_Color focusedColor{255, 255, 0, 255};

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(50, 150);
        go->AddComponent<dae::TextComponent>("Single Player", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::ButtonComponent>(
            [&]() { dae::SceneManager::GetInstance().LoadScene<dae::GameSceneLoader>(true); }, focusedColor, idleColor);
        singlePlayerButton = go->GetComponent<dae::ButtonComponent>();
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(50, 200);
        go->AddComponent<dae::TextComponent>("Coop", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::ButtonComponent>(
            [&]() { dae::SceneManager::GetInstance().LoadScene<dae::GameSceneLoader>(true); }, focusedColor, idleColor);
        coopButton = go->GetComponent<dae::ButtonComponent>();
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(50, 250);
        go->AddComponent<dae::TextComponent>("Pvp", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::ButtonComponent>(
            [&]() { dae::SceneManager::GetInstance().LoadScene<dae::GameSceneLoader>(true); }, focusedColor, idleColor);
        pvpButton = go->GetComponent<dae::ButtonComponent>();
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(50, 300);
        go->AddComponent<dae::TextComponent>("Quit", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::ButtonComponent>(
            [&]() {
                SDL_Event quit_event;
                quit_event.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&quit_event);
            },
            focusedColor, idleColor);
        quitButton = go->GetComponent<dae::ButtonComponent>();
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

        controllerInput0->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Down), InputKeybinds::DPAD_DOWN,
            InputState::JustPressed);

        controllerInput0->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Up), InputKeybinds::DPAD_UP,
            InputState::JustPressed);

        controllerInput0->AddBinding(
            std::make_unique<dae::PressButtonCommand>(), InputKeybinds::BUTTON_SOUTH, InputState::JustPressed);

        controllerInput1->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Down), InputKeybinds::DPAD_DOWN,
            InputState::JustPressed);

        controllerInput1->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Up), InputKeybinds::DPAD_UP,
            InputState::JustPressed);

        controllerInput1->AddBinding(
            std::make_unique<dae::PressButtonCommand>(), InputKeybinds::BUTTON_SOUTH, InputState::JustPressed);

        keyboardInput->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Up), InputKeybinds::W, InputState::JustPressed);

        keyboardInput->AddBinding(
            std::make_unique<dae::NavigateButtonCommand>(Direction::Down), InputKeybinds::S, InputState::JustPressed);

        keyboardInput->AddBinding(
            std::make_unique<dae::PressButtonCommand>(), InputKeybinds::Z, InputState::JustPressed);
    }
    return scene;
}
