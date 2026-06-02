#include "GameSceneLoader.hpp"

#include "Binding.hpp"
#include "CustomCommands.hpp"
#include "FPSDisplay.hpp"
#include "GameObject.hpp"
#include "Health.hpp"
#include "InputManager.hpp"
#include "LevelGrid.hpp"
#include "LivesDisplay.hpp"
#include "PlayerAnimation.hpp"
#include "PlayerController.hpp"
#include "ObjectRenderer.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"
#include "Score.hpp"
#include "ScoreDisplay.hpp"
#include "Subject.hpp"
#include "TextDisplay.hpp"
#include "burgerLayer.hpp"

#include <fstream>
#include <sstream>

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
        go->AddComponent<dae::LevelGrid>(tileSize, "Data/Levels/Level0.csv", charToTexture);
        levelGrid = go->GetComponent<LevelGrid>();
        level = go.get();
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(292, 0);
        go->AddComponent<dae::TextDisplay>("Programming 4 Assignment", fontMain, SDL_Color{255, 0, 0, 255});
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->AddComponent<dae::TextDisplay>("60.0", fontMain, SDL_Color{255, 0, 0, 255});
        go->AddComponent<dae::FpsDisplay>();
        scene->Add(std::move(go));
    }
    auto player1 = std::make_unique<GameObject>();
    auto player2 = std::make_unique<GameObject>();
    // create 2 players
    {
        player2->AddComponent<dae::ObjectRenderer>();
        player2->AddComponent<dae::Health>(std::make_unique<dae::Subject>(), levelInfo.lifeCount);
        player2->AddComponent<dae::Texture2DDisplay>("Data/pepperguy.png", 32, 32);
        player2->GetComponent<dae::Transform>()->SetLocalPosition(glm::vec3{200, 214, 0});
        player2->AddComponent<dae::Score>(std::make_unique<dae::Subject>());
        player2->AddComponent<dae::PlayerAnimation>(
            "Data/Characters/PepperGuy_AnimationData.json", "Data/Characters/PepperGuy_SpriteSheet.png");
        player2->AddComponent<dae::PlayerController>(
            inputManager.GetKeyboardInput(), level->GetComponent<dae::LevelGrid>());

        player1->AddComponent<dae::ObjectRenderer>();
        player1->AddComponent<dae::Health>(std::make_unique<dae::Subject>(), levelInfo.lifeCount);
        player1->AddComponent<dae::Texture2DDisplay>("Data/pepperguy.png", 32, 32);
        player1->GetComponent<dae::Transform>()->SetLocalPosition(glm::vec3{150, 214, 0});
        player1->AddComponent<dae::Score>(std::make_unique<dae::Subject>());
        player1->AddComponent<dae::PlayerAnimation>(
            "Data/Characters/PepperGuy_AnimationData.json", "Data/Characters/PepperGuy_SpriteSheet.png");
        player1->AddComponent<dae::PlayerController>(
            inputManager.GetControllerInput(0), level->GetComponent<dae::LevelGrid>());
    }

    // add player bindings
    dae::Subject *player1PickUpSubject1{nullptr};
    dae::Subject *player1PickUpSubject2{nullptr};
    dae::Subject *player2PickUpSubject1{nullptr};
    dae::Subject *player2PickUpSubject2{nullptr};
    {
        std::vector<Binding *> bindings{};
        auto player1Input{inputManager.GetControllerInput(0)};
        auto player2Input{inputManager.GetKeyboardInput()};

        auto player1DamageBinding = player1Input->AddBinding(
            std::make_unique<dae::DamagePlayer>(*player1, *player2), InputKeybinds::BUTTON_WEST,
            InputState::JustPressed);
        auto player1PickUpBinding1 = player1Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player1), InputKeybinds::BUTTON_SOUTH, InputState::JustPressed);
        auto player1PickUpBinding2 = player1Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player1), InputKeybinds::BUTTON_EAST, InputState::JustPressed);

        bindings.push_back(player1DamageBinding);
        bindings.push_back(player1PickUpBinding1);
        bindings.push_back(player1PickUpBinding2);

        auto player2DamageBinding = player2Input->AddBinding(
            std::make_unique<dae::DamagePlayer>(*player2, *player1), InputKeybinds::C, InputState::JustPressed);
        auto player2PickUpBinding1 = player2Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player2), InputKeybinds::Z, InputState::JustPressed);
        auto player2PickUpBinding2 = player2Input->AddBinding(
            std::make_unique<dae::PickUpItemCommand>(*player2), InputKeybinds::X, InputState::JustPressed);

        bindings.push_back(player2DamageBinding);
        bindings.push_back(player2PickUpBinding1);
        bindings.push_back(player2PickUpBinding2);

        player1PickUpSubject1 =
            static_cast<dae::PickUpItemCommand *>(player1PickUpBinding1->m_Command.get())->GetSubject();
        player1PickUpSubject2 =
            static_cast<dae::PickUpItemCommand *>(player1PickUpBinding2->m_Command.get())->GetSubject();
        player2PickUpSubject1 =
            static_cast<dae::PickUpItemCommand *>(player2PickUpBinding1->m_Command.get())->GetSubject();
        player2PickUpSubject2 =
            static_cast<dae::PickUpItemCommand *>(player2PickUpBinding2->m_Command.get())->GetSubject();

        player1PickUpSubject1->AddObserver(player1->GetComponent<dae::Score>());
        player1PickUpSubject2->AddObserver(player1->GetComponent<dae::Score>());
        player2PickUpSubject1->AddObserver(player2->GetComponent<dae::Score>());
        player2PickUpSubject2->AddObserver(player2->GetComponent<dae::Score>());

        scene->AddExitFunction([bindings]() {
            auto &inputManager{InputManager::GetInstance()};
            auto keyBoardInput{inputManager.GetKeyboardInput()};
            std::vector<PlayerInput *> controllerInputs{
                inputManager.GetControllerInput(0), inputManager.GetControllerInput(1),
                inputManager.GetControllerInput(2), inputManager.GetControllerInput(3)};

            for (auto const &binding : bindings) {
                keyBoardInput->UnBind(binding);
                for (auto const &controllerInput : controllerInputs)
                    controllerInput->UnBind(binding);
            }
        });
    }
    // instructions
    {
        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(0, 650);
        go->AddComponent<dae::TextDisplay>(
            "Use the D-Pad to move Peter Pepper 1, X to inflict damage, A and "
            "B to "
            "collect points",
            fontSmall);
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(0, 710);
        go->AddComponent<dae::TextDisplay>(
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
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(920, 650);
        go->AddComponent<dae::TextDisplay>("temp", fontSmall);
        go->AddComponent<dae::LivesDisplay>(*player1->GetComponent<dae::Health>());
        auto livesLostEvent = player1->GetComponent<dae::Health>()->GetSubject();
        livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplay>());
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(920, 675);
        go->AddComponent<dae::TextDisplay>("Score: 0", fontSmall);
        go->AddComponent<dae::ScoreDisplay>(*player1->GetComponent<dae::Score>());
        player1->GetComponent<dae::Score>()->GetSubject()->AddObserver(go->GetComponent<dae::ScoreDisplay>());
        scene->Add(std::move(go));

        // player 2
        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(920, 710);
        go->AddComponent<dae::TextDisplay>("temp", fontSmall);
        go->AddComponent<dae::LivesDisplay>(*player2->GetComponent<dae::Health>());
        livesLostEvent = player2->GetComponent<dae::Health>()->GetSubject();
        livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplay>());
        scene->Add(std::move(go));

        go = std::make_unique<dae::GameObject>();
        go->AddComponent<dae::ObjectRenderer>();
        go->GetComponent<dae::Transform>()->SetLocalPosition(920, 735);
        go->AddComponent<dae::TextDisplay>("Score: 0", fontSmall);
        go->AddComponent<dae::ScoreDisplay>(*player2->GetComponent<dae::Score>());
        player2->GetComponent<dae::Score>()->GetSubject()->AddObserver(go->GetComponent<dae::ScoreDisplay>());
        scene->Add(std::move(go));
    }

    LoadSpriteMap(scene, tileSize, std::vector<GameObject *>{player1.get(), player2.get()}, levelGrid);

    scene->Add(std::move(player1));
    scene->Add(std::move(player2));
    return scene;
}

void dae::GameSceneLoader::LoadSpriteMap(
    Scene *scene, glm::ivec2 const &tileSize, std::vector<GameObject *> const &players, LevelGrid *levelGrid) {
    std::string const &filePath{"Data/Levels/Level0_Burgers.csv"};
    std::ifstream stream{filePath};
    std::string line;
    glm::ivec2 gridCoord{};
    std::unordered_map<char, BurgerLayerType> layers{
        {'0', BurgerLayerType::TopPaddy},
        {'1', BurgerLayerType::Tomato},
        {'2', BurgerLayerType::Salad},
        {'3', BurgerLayerType::BottomPaddy},
    };

    std::unordered_map<char, GameObject *> charToPlayers;
    for (uint32_t i = 0; i < players.size(); i++) {
        charToPlayers[char('a' + i)] = players[i];
    }
    BurgerLayer::AllBurgerLayers.clear();
    while (std::getline(stream, line)) {
        std::stringstream ss{line};
        std::string cell;

        // goes over every character between ','
        while (std::getline(ss, cell, ',')) {
            char currentChar{cell[0]};
            if (charToPlayers.contains(currentChar)) {
                glm::vec2 pos{gridCoord * tileSize + tileSize / 4};
                pos.y += tileSize.y / 8;
                auto playerTransform = charToPlayers.at(currentChar)->GetComponent<Transform>();
                playerTransform->SetLocalPosition(pos);
            }
            if (layers.contains(currentChar)) {
                glm::vec2 pos{gridCoord * tileSize};
                // to place them at exactly the right spot since they don't have the same size as the tiles
                pos.x += tileSize.x / 3.5f;
                pos.y += tileSize.y / 1.9f;
                auto go = std::make_unique<GameObject>();
                go->AddComponent<dae::ObjectRenderer>();
                go->GetComponent<dae::Transform>()->SetLocalPosition(pos);
                go->AddComponent<dae::BurgerLayer>(layers.at(currentChar), players, levelGrid);
                scene->Add(std::move(go));
            }
            gridCoord.x++;
        }
        gridCoord.x = 0;
        gridCoord.y++;
    }
}
