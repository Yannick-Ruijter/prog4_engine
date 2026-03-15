#include "BurgerTime.h"
#include "FPSComponent.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Command.h"
#include "Binding.h"
#include "InputInfo.h"
#include "Subject.h"
#include "PlayerLivesObserver.h"
#include "HealthComponent.h"

BurgerTime::BurgerTime() = default;

BurgerTime::~BurgerTime() = default;

void BurgerTime::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& inputManager = dae::InputManager::GetInstance();
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::Texture2DComponent>("Data/background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180);
	go->AddComponent<dae::Texture2DComponent>("Data/logo.png");
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20);
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, SDL_Color{ 255, 0, 0, 255 });
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("60.0", font, SDL_Color{ 255, 0, 0, 255 });
	go->AddComponent<dae::FpsComponent>();
	scene.Add(std::move(go));



	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 120);
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, SDL_Color{ 255, 0, 0, 255 });
	dae::TextComponent* textComponent = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	m_Player1HealthObserver = std::move(std::make_unique<dae::PlayerLivesObserver>(textComponent));
	auto livesLostEvent{ std::make_unique<dae::Subject>() };
	livesLostEvent->AddObserver(m_Player1HealthObserver.get());
	go->AddComponent<dae::HealthComponent>(std::move(livesLostEvent));
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Up),
		InputKeybinds::W, InputState::Pressed);
	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Down),
		InputKeybinds::S, InputState::Pressed);
	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Left),
		InputKeybinds::A, InputState::Pressed);
	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Right),
		InputKeybinds::D, InputState::Pressed);
	m_Player2 = go.get();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::HealthComponent>();
	go->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Up, 200.f),
		InputKeybinds::DPAD_UP, InputState::Pressed);
	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Down, 200.f),
		InputKeybinds::DPAD_DOWN, InputState::Pressed);
	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Left, 200.f),
		InputKeybinds::DPAD_LEFT, InputState::Pressed);
	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go.get(), dae::MoveDirection::Right, 200.f),
		InputKeybinds::DPAD_RIGHT, InputState::Pressed);
	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::DamagePlayer>(*go.get(), *m_Player2)
		, InputKeybinds::X, InputState::JustPressed);

	m_Player1 = go.get();
	scene.Add(std::move(go));
}