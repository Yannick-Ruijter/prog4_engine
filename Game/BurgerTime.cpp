#include "BurgerTime.h"
#include "FPSComponent.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "MoveComponent.h"
#include "InputManager.h"
#include "Command.h"

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
	go->AddComponent<dae::TextComponent>("Test", font, SDL_Color{ 255, 0, 0, 255 });
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });
	go->AddComponent<dae::MoveComponent>();
	inputManager.AddCommand(std::make_unique<dae::MoveObjectCommand>(*go.get()));
	scene.Add(std::move(go));

}