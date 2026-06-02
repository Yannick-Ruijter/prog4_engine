#include "BurgerTime.hpp"

#include "CustomCommands.hpp"
#include "GameSceneLoader.hpp"
#include "InputManager.hpp"
#include "MainSceneLoader.hpp"
#include "ServiceProvider.hpp"
// Create a gameOverScene
// display score and gamemode
// upon completely dying, ask if they want to save their score
// If yes, make a visual keyboard pop up (they're all just buttons)
// and then once they save, write it to a file
// in all cases, we send them back to the main menu when the game is over

BurgerTime::BurgerTime() = default;

BurgerTime::~BurgerTime() = default;

void BurgerTime::Initialize() {
    dae::ServiceProvider::RegisterSoundSystem(
        std::make_unique<dae::SDL_SoundSystem>(std::vector<std::pair<sound_id, std::string>>{
            {0, "Data/Sounds/Coin.wav"},
            {1, "Data/Sounds/Pepper Shake.wav"},
            {2, "Data/Sounds/BGM.wav"},
            {3, "Data/Sounds/Burger Step.wav"},
            {4, "Data/Sounds/Burger Fall.wav"},
            {5, "Data/Sounds/Burger Land.wav"},
            {6, "Data/Sounds/Death.wav"},
        }));
    SetupKeybinds();

    // first argument: set scene as active scene
    dae::SceneManager::GetInstance().LoadScene<dae::MainSceneLoader>();
}

void BurgerTime::Destroy() {
    dae::ServiceProvider::GetSoundSystem().Destroy();
}

void BurgerTime::SetupKeybinds() {
    auto &inputManager = dae::InputManager::GetInstance();
    inputManager.GetKeyboardInput()
        ->BindInputAction(InputAction::MoveUp, InputKeybinds::UP)
        .BindInputAction(InputAction::MoveDown, InputKeybinds::DOWN)
        .BindInputAction(InputAction::MoveRight, InputKeybinds::RIGHT)
        .BindInputAction(InputAction::MoveLeft, InputKeybinds::LEFT)
        .BindInputAction(InputAction::Attack, InputKeybinds::W);

    for (int i = 0; i < 4; ++i) {
        inputManager.GetControllerInput(i)
            ->BindInputAction(InputAction::MoveUp, InputKeybinds::DPAD_UP)
            .BindInputAction(InputAction::MoveDown, InputKeybinds::DPAD_DOWN)
            .BindInputAction(InputAction::MoveRight, InputKeybinds::DPAD_RIGHT)
            .BindInputAction(InputAction::MoveLeft, InputKeybinds::DPAD_LEFT)
            .BindInputAction(InputAction::Attack, InputKeybinds::BUTTON_SOUTH);
    }

    auto keyboardInput{inputManager.GetKeyboardInput()};
    keyboardInput->AddBinding(std::make_unique<dae::ToggleSoundCommand>(), InputKeybinds::F2, InputState::JustPressed);
}
