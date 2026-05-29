#include "BurgerTime.hpp"

#include "Achievements.hpp"
#include "Binding.hpp"
#include "Component.hpp"
#include "CustomCommands.hpp"
#include "FPSComponent.hpp"
#include "GameSceneLoader.hpp"
#include "HealthComponent.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "LevelGridComponent.hpp"
#include "LivesDisplayComponent.hpp"
#include "MainSceneLoader.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerComponent.hpp"
#include "PlayerLivesObserver.hpp"
#include "PlayerState.hpp"
#include "PlayerStateIdle.hpp"
#include "RectColliderComponent.hpp"
#include "RenderComponent.hpp"
#include "ResourceManager.hpp"
#include "SDL_SoundSystem.hpp"
#include "ScoreComponent.hpp"
#include "ScoreDisplayComponent.hpp"
#include "ServiceProvider.hpp"
#include "Subject.hpp"
#include "TextComponent.hpp"
#include "Texture2DComponent.hpp"
#include "TransformComponent.hpp"
#include "sdbm_hash.hpp"
#include <map>
#include <tuple>

// TODO: Add function for setting up all the scenes
// create a resetScene function that has to be called whenever they restart from the main scene
// make the gamescene take the gamemode as param (singleplayer, coop or vs)
//  create a gamesceneloader class
//  I can then make the LoadScene function take a sceneloader as argument
//  and take from that sceneloader the actual scene
//  the gamesceneloader has static members such as player lives, currentscore and currentlevel so that they don't reset
//  upon reloading scene Create a class for each scene
//
//
// Create a mainsceneLoader class which also create the main scene
// find a font for it
// create a MenuScreenState class
// create a command for moving to the next button
// it has the currently selected button
// a button has a OnCurrentButton function which changes the visuals
// a button has a visual for onNotCurrentButton function (choose other names)
// which assigns a different visual for the button
// each button has for each direction a next button
// once the quit button was pressed quit
// once a mode has been chosen, load the gamescene
//
// Create a gameOverScene
// display score and gamemode
// upon completely dying, ask if they want to save their score
// If yes, make a visual keyboard pop up (they're all just buttons)
// and then once they save, write it to a file
// in all cases, we send them back to the main menu when the game is over

BurgerTime::BurgerTime() = default;

BurgerTime::~BurgerTime() = default;

void BurgerTime::Initialize()
{
    dae::ServiceProvider::RegisterSoundSystem(
        std::make_unique<dae::SDL_SoundSystem>(std::vector<std::pair<sound_id, std::string>>{
            {0, "Data/Sounds/Coin.wav"}, {1, "Data/Sounds/Pepper Shake.wav"}}));
    SetupKeybinds();

    // first argument: set scene as active scene
    dae::SceneManager::GetInstance().LoadScene<dae::GameSceneLoader>(false);
    dae::SceneManager::GetInstance().LoadScene<dae::MainSceneLoader>(true);
}

void BurgerTime::Destroy()
{
    dae::ServiceProvider::GetSoundSystem().Destroy();
}

void BurgerTime::SetupKeybinds()
{
    auto &inputManager = dae::InputManager::GetInstance();
    inputManager.GetKeyboardInput()
        ->BindInputAction(InputAction::MoveUp, InputKeybinds::W)
        .BindInputAction(InputAction::MoveDown, InputKeybinds::S)
        .BindInputAction(InputAction::MoveRight, InputKeybinds::D)
        .BindInputAction(InputAction::MoveLeft, InputKeybinds::A);

    for (int i = 0; i < 4; ++i)
    {
        inputManager.GetControllerInput(i)
            ->BindInputAction(InputAction::MoveUp, InputKeybinds::DPAD_UP)
            .BindInputAction(InputAction::MoveDown, InputKeybinds::DPAD_DOWN)
            .BindInputAction(InputAction::MoveRight, InputKeybinds::DPAD_RIGHT)
            .BindInputAction(InputAction::MoveLeft, InputKeybinds::DPAD_LEFT);
    }
}
