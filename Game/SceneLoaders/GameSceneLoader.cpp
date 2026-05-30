#include "Binding.hpp"
#include "CustomCommands.hpp"
#include "FPSComponent.hpp"
#include "GameSceneLoader.hpp"
#include "HealthComponent.hpp"
#include "InputManager.hpp"
#include "LevelGridComponent.hpp"
#include "LivesDisplayComponent.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerComponent.hpp"
#include "RenderComponent.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "ScoreComponent.hpp"
#include "ScoreDisplayComponent.hpp"
#include "Subject.hpp"
#include "TextComponent.hpp"
#include "burgerLayerComponent.hpp"
#include <fstream>

using namespace dae;

Scene *dae::GameSceneLoader::LoadScene(LevelInfo levelInfo) {
    auto scene = dae::SceneManager::GetInstance().CreateScene();
    // dae::SceneManager::GetInstance().SetActiveScene(scene);
    auto &inputManager = dae::InputManager::GetInstance();
    auto go = std::make_unique<dae::GameObject>();
    auto fontMain = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

    dae::GameObject *level{};
    glm::ivec2 const tileSize{64, 64};
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

        go->AddComponent<dae::RenderComponent>();
        go->AddComponent<dae::LevelGridComponent>(tileSize, "Data/Levels/Level0.csv", charToTexture);
        level = go.get();
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 0);
        go->AddComponent<dae::TextComponent>("Programming 4 Assignment", fontMain, SDL_Color{255, 0, 0, 255});
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->AddComponent<dae::TextComponent>("60.0", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::FpsComponent>();
        scene->Add(std::move(go));
    }
    auto player1 = std::make_unique<GameObject>();
    auto player2 = std::make_unique<GameObject>();
    // create 2 players
    {
        player2->AddComponent<dae::RenderComponent>();
        player2->AddComponent<dae::HealthComponent>(std::make_unique<dae::Subject>(), levelInfo.lifeCount);
        player2->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png", 32, 32);
        player2->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{200, 214, 0});
        player2->AddComponent<dae::ScoreComponent>(std::make_unique<dae::Subject>());
        player2->AddComponent<dae::PlayerAnimationComponent>(
            "Data/Characters/PepperGuy_AnimationData.json", "Data/Characters/PepperGuy_SpriteSheet.png");
        player2->AddComponent<dae::PlayerComponent>(
            inputManager.GetKeyboardInput(), level->GetComponent<dae::LevelGridComponent>());

        player1->AddComponent<dae::RenderComponent>();
        player1->AddComponent<dae::HealthComponent>(std::make_unique<dae::Subject>(), levelInfo.lifeCount);
        player1->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png", 32, 32);
        player1->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{150, 214, 0});
        player1->AddComponent<dae::ScoreComponent>(std::make_unique<dae::Subject>());
        player1->AddComponent<dae::PlayerAnimationComponent>(
            "Data/Characters/PepperGuy_AnimationData.json", "Data/Characters/PepperGuy_SpriteSheet.png");
        player1->AddComponent<dae::PlayerComponent>(
            inputManager.GetControllerInput(0), level->GetComponent<dae::LevelGridComponent>());
    }

    // add player bindings
    dae::Subject *player1PickUpSubject1{nullptr};
    dae::Subject *player1PickUpSubject2{nullptr};
    dae::Subject *player2PickUpSubject1{nullptr};
    dae::Subject *player2PickUpSubject2{nullptr};
    {
        auto player1Input{inputManager.GetControllerInput(0)};
        auto player2Input{inputManager.GetKeyboardInput()};

        auto player1DamageBinding = player1Input->AddBinding(
            std::make_unique<dae::DamagePlayer>(*player1, *player2), InputKeybinds::BUTTON_WEST,
            InputState::JustPressed);
        auto player1PickUpBinding1 = player1Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player1), InputKeybinds::BUTTON_SOUTH, InputState::JustPressed);
        auto player1PickUpBinding2 = player1Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player1), InputKeybinds::BUTTON_EAST, InputState::JustPressed);

        scene->AddBinding(player1DamageBinding, player1Input);
        scene->AddBinding(player1PickUpBinding1, player1Input);
        scene->AddBinding(player1PickUpBinding2, player1Input);

        auto player2DamageBinding = player2Input->AddBinding(
            std::make_unique<dae::DamagePlayer>(*player2, *player1), InputKeybinds::C, InputState::JustPressed);
        auto player2PickUpBinding1 = player2Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player2), InputKeybinds::Z, InputState::JustPressed);
        auto player2PickUpBinding2 = player2Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player2), InputKeybinds::X, InputState::JustPressed);

        scene->AddBinding(player2DamageBinding, player2Input);
        scene->AddBinding(player2PickUpBinding1, player2Input);
        scene->AddBinding(player2PickUpBinding2, player2Input);

        player1PickUpSubject1 =
            static_cast<dae::PickUpItemCommand *>(player1PickUpBinding1->m_Command.get())->GetSubject();
        player1PickUpSubject2 =
            static_cast<dae::PickUpItemCommand *>(player1PickUpBinding2->m_Command.get())->GetSubject();
        player2PickUpSubject1 =
            static_cast<dae::PickUpItemCommand *>(player2PickUpBinding1->m_Command.get())->GetSubject();
        player2PickUpSubject2 =
            static_cast<dae::PickUpItemCommand *>(player2PickUpBinding2->m_Command.get())->GetSubject();

        player1PickUpSubject1->AddObserver(player1->GetComponent<dae::ScoreComponent>());
        player1PickUpSubject2->AddObserver(player1->GetComponent<dae::ScoreComponent>());
        player2PickUpSubject1->AddObserver(player2->GetComponent<dae::ScoreComponent>());
        player2PickUpSubject2->AddObserver(player2->GetComponent<dae::ScoreComponent>());
    }
    // instructions
    {
        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 650);
        go->AddComponent<dae::TextComponent>(
            "Use the D-Pad to move Peter Pepper 1, X to inflict damage, A and "
            "B to "
            "collect points",
            fontSmall);
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 710);
        go->AddComponent<dae::TextComponent>(
            "Use the WASD to move Peter Pepper 2, C to inflict damage, Z and X "
            "to "
            "collect points",
            fontSmall);
        scene->Add(std::move(go));
    }
    // lives display
    {
        // player 1
        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(920, 650);
        go->AddComponent<dae::TextComponent>("temp", fontSmall);
        go->AddComponent<dae::LivesDisplayComponent>(*player1->GetComponent<dae::HealthComponent>());
        auto livesLostEvent = player1->GetComponent<dae::HealthComponent>()->GetSubject();
        livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplayComponent>());
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(920, 675);
        go->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
        go->AddComponent<dae::ScoreDisplayComponent>(*player1->GetComponent<dae::ScoreComponent>());
        player1->GetComponent<dae::ScoreComponent>()->GetSubject()->AddObserver(
            go->GetComponent<dae::ScoreDisplayComponent>());
        scene->Add(std::move(go));

        // player 2
        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(920, 710);
        go->AddComponent<dae::TextComponent>("temp", fontSmall);
        go->AddComponent<dae::LivesDisplayComponent>(*player2->GetComponent<dae::HealthComponent>());
        livesLostEvent = player2->GetComponent<dae::HealthComponent>()->GetSubject();
        livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplayComponent>());
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::RenderComponent>();
        go->GetComponent<dae::TransformComponent>()->SetLocalPosition(920, 735);
        go->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
        go->AddComponent<dae::ScoreDisplayComponent>(*player2->GetComponent<dae::ScoreComponent>());
        player2->GetComponent<dae::ScoreComponent>()->GetSubject()->AddObserver(
            go->GetComponent<dae::ScoreDisplayComponent>());
        scene->Add(std::move(go));
    }

    LoadSpriteMap(scene, tileSize, std::vector<GameObject *>{player1.get(), player2.get()});

    scene->Add(std::move(player1));
    scene->Add(std::move(player2));
    return scene;
}

void dae::GameSceneLoader::LoadSpriteMap(
    Scene *scene, glm::ivec2 const &tileSize, std::vector<GameObject *> const &players) {
    std::ifstream stream{"Data/Levels/Level0_Burgers.csv"};
    std::string line;
    glm::ivec2 gridCoord{};
    std::unordered_map<char, BurgerLayer> layers{
        {'0', BurgerLayer::TopPaddy},
        {'1', BurgerLayer::Cheese},
        {'2', BurgerLayer::Salad},
        {'3', BurgerLayer::BottomPaddy},
    };

    std::unordered_map<char, GameObject *> charToPlayers;
    for (int i = 0; i < players.size(); i++) {
        charToPlayers[char('a' + i)] = players[i];
    }

    while (std::getline(stream, line)) {
        std::stringstream ss{line};
        std::string cell;

        // goes over every character between ','
        while (std::getline(ss, cell, ',')) {
            char currentChar{cell[0]};
            if (charToPlayers.contains(currentChar)) {
                glm::vec2 pos{gridCoord * tileSize + tileSize / 4};
                auto playerTransform = charToPlayers.at(currentChar)->GetComponent<TransformComponent>();
                playerTransform->SetLocalPosition(pos);
            }
            if (layers.contains(currentChar)) {
                glm::vec2 pos{gridCoord * tileSize};
                pos.x += tileSize.x / 3;
                pos.y += tileSize.y / 1.75f;
                auto go = std::make_unique<GameObject>();
                go->AddComponent<dae::RenderComponent>();
                go->GetComponent<dae::TransformComponent>()->SetLocalPosition(pos);
                go->AddComponent<dae::BurgerLayerComponent>(layers.at(currentChar), players);
                scene->Add(std::move(go));
            }
            gridCoord.x++;
        }
        gridCoord.x = 0;
        gridCoord.y++;
    }
}
