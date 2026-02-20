#include "Game.h"
#include "FPSComponent.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"

Game::Game()
{
	
}

void Game::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TransformComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::RenderComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::Texture2DComponent>(*go.get(), "Data/background.png"));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::TransformComponent>(*go.get()));
	go->GetComponent<dae::TransformComponent>()->SetPosition(358, 180);
	go->AddComponent(std::make_unique<dae::Texture2DComponent>(*go.get(), "Data/logo.png"));
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::TransformComponent>(*go.get()));
	go->GetComponent<dae::TransformComponent>()->SetPosition(292, 20);
	go->AddComponent(std::make_unique<dae::TextComponent>(*go.get(), "Programming 4 Assignment", font, SDL_Color{255, 0, 0, 255}));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::TransformComponent>(*go.get()));
	go->AddComponent(std::make_unique<dae::TextComponent>(*go.get(), "60.0", font, SDL_Color{ 255, 0, 0, 255 }));
	go->AddComponent(std::make_unique<dae::FpsComponent>(*go.get()));
	scene.Add(std::move(go));
}

void Game::Update(float) {}

void Game::Render() const {}