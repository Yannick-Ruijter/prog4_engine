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
