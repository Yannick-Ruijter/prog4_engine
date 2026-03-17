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
#include "LivesDisplayComponent.h"
#include "Binding.h"
#include "ScoreDisplayComponent.h"
#include "ScoreComponent.h"
BurgerTime::BurgerTime() = default;

BurgerTime::~BurgerTime() = default;

void BurgerTime::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& inputManager = dae::InputManager::GetInstance();
	auto go = std::make_unique<dae::GameObject>();
	auto fontMain = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	//background stuff
	{
		go->AddComponent<dae::RenderComponent>();
		go->AddComponent<dae::Texture2DComponent>("Data/background.png");
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180);
		go->AddComponent<dae::Texture2DComponent>("Data/logo.png");
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20);
		go->AddComponent<dae::TextComponent>("Programming 4 Assignment", fontMain, SDL_Color{ 255, 0, 0, 255 });
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->AddComponent<dae::TextComponent>("60.0", fontMain, SDL_Color{ 255, 0, 0, 255 });
		go->AddComponent<dae::FpsComponent>();
		scene.Add(std::move(go));
	}
	//create 2 players
	{
		go = std::make_unique<dae::GameObject>();
		m_Player2 = go.get();
		m_Player2->AddComponent<dae::RenderComponent>();
		m_Player2->AddComponent<dae::HealthComponent>(std::make_unique<dae::Subject>(), m_StartingLives);
		m_Player2->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png");
		m_Player2->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });
		m_Player2->AddComponent<dae::ScoreComponent>(std::make_unique<dae::Subject>());
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		m_Player1 = go.get();
		m_Player1->AddComponent<dae::RenderComponent>();
		m_Player1->AddComponent<dae::HealthComponent>(std::make_unique<dae::Subject>(), m_StartingLives);
		m_Player1->AddComponent<dae::Texture2DComponent>("Data/pepperguy.png");
		m_Player1->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });
		m_Player1->AddComponent<dae::ScoreComponent>(std::make_unique<dae::Subject>());
		scene.Add(std::move(go));
	}

	//add player bindings
	dae::Subject* player1PickUpSubject1{ nullptr };
	dae::Subject* player1PickUpSubject2{ nullptr };
	dae::Subject* player2PickUpSubject1{ nullptr };
	dae::Subject* player2PickUpSubject2{ nullptr };
	{
		inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player2, dae::MoveDirection::Up),
			InputKeybinds::W, InputState::Pressed);
		inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player2, dae::MoveDirection::Down),
			InputKeybinds::S, InputState::Pressed);
		inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player2, dae::MoveDirection::Left),
			InputKeybinds::A, InputState::Pressed);
		inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player2, dae::MoveDirection::Right),
			InputKeybinds::D, InputState::Pressed);
		inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::DamagePlayer>(*m_Player2, *m_Player1)
			, InputKeybinds::C, InputState::JustPressed);
		dae::Binding* player2PickUpBinding1 = inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::PickUpItemCommand>(*m_Player2)
			, InputKeybinds::Z, InputState::JustPressed);
		dae::Binding* player2PickUpBinding2 = inputManager.GetKeyboardInput()->AddBinding(
			std::make_unique<dae::PickUpItemCommand>(*m_Player2)
			, InputKeybinds::X, InputState::JustPressed);


		inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player1, dae::MoveDirection::Up, 200.f),
			InputKeybinds::DPAD_UP, InputState::Pressed);
		inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player1, dae::MoveDirection::Down, 200.f),
			InputKeybinds::DPAD_DOWN, InputState::Pressed);
		inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player1, dae::MoveDirection::Left, 200.f),
			InputKeybinds::DPAD_LEFT, InputState::Pressed);
		inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::MoveObjectCommand>(*m_Player1, dae::MoveDirection::Right, 200.f),
			InputKeybinds::DPAD_RIGHT, InputState::Pressed);
		inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::DamagePlayer>(*m_Player1, *m_Player2)
			, InputKeybinds::BUTTON_WEST, InputState::JustPressed);
		dae::Binding* player1PickUpBinding1 = inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::PickUpItemCommand>(*m_Player1)
			, InputKeybinds::BUTTON_SOUTH, InputState::JustPressed);
		dae::Binding* player1PickUpBinding2 = inputManager.GetControllerInput(0)->AddBinding(
			std::make_unique<dae::PickUpItemCommand>(*m_Player1)
			, InputKeybinds::BUTTON_EAST, InputState::JustPressed);

		player1PickUpSubject1 = static_cast<dae::PickUpItemCommand*>(player1PickUpBinding1->m_Command.get())->GetSubject();
		player1PickUpSubject2 = static_cast<dae::PickUpItemCommand*>(player1PickUpBinding2->m_Command.get())->GetSubject();
		player2PickUpSubject1 = static_cast<dae::PickUpItemCommand*>(player2PickUpBinding1->m_Command.get())->GetSubject();
		player2PickUpSubject2 = static_cast<dae::PickUpItemCommand*>(player2PickUpBinding2->m_Command.get())->GetSubject();

		player1PickUpSubject1->AddObserver(m_Player1->GetComponent<dae::ScoreComponent>());
		player1PickUpSubject2->AddObserver(m_Player1->GetComponent<dae::ScoreComponent>());
		player2PickUpSubject1->AddObserver(m_Player2->GetComponent<dae::ScoreComponent>());
		player2PickUpSubject2->AddObserver(m_Player2->GetComponent<dae::ScoreComponent>());
	}
	//instructions
	{
		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 90);
		go->AddComponent<dae::TextComponent>("Use the D-Pad to move Peter Pepper 1, X to inflict damage, A and B to collect points", fontSmall);
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 110);
		go->AddComponent<dae::TextComponent>("Use the WASD to move Peter Pepper 2, C to inflict damage, Z and X to collect points", fontSmall);
		scene.Add(std::move(go));
	}
	//lives display
	{
		//player 1
		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 150);
		go->AddComponent<dae::TextComponent>("temp", fontSmall);
		go->AddComponent<dae::LivesDisplayComponent>(*m_Player1->GetComponent<dae::HealthComponent>());
		auto livesLostEvent = m_Player1->GetComponent<dae::HealthComponent>()->GetSubject();
		livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplayComponent>());
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 175);
		go->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
		go->AddComponent<dae::ScoreDisplayComponent>(*m_Player1->GetComponent<dae::ScoreComponent>());
		m_Player1->GetComponent< dae::ScoreComponent>()->GetSubject()->AddObserver(go->GetComponent<dae::ScoreDisplayComponent>());
		scene.Add(std::move(go));

		//player 2
		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 200);
		go->AddComponent<dae::TextComponent>("temp", fontSmall);
		go->AddComponent<dae::LivesDisplayComponent>(*m_Player2->GetComponent<dae::HealthComponent>());
		livesLostEvent = m_Player2->GetComponent<dae::HealthComponent>()->GetSubject();
		livesLostEvent->AddObserver(go->GetComponent<dae::LivesDisplayComponent>());
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::RenderComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 225);
		go->AddComponent<dae::TextComponent>("Score: 0", fontSmall);
		go->AddComponent<dae::ScoreDisplayComponent>(*m_Player2->GetComponent<dae::ScoreComponent>());
		m_Player2->GetComponent< dae::ScoreComponent>()->GetSubject()->AddObserver(go->GetComponent< dae::ScoreDisplayComponent>());
		scene.Add(std::move(go));
	}

}