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
	dae::TransformComponent::AddToGameObject(go.get());
	dae::RenderComponent::AddToGameObject(go.get());
	dae::Texture2DComponent::AddToGameObject(go.get(), "background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	dae::RenderComponent::AddToGameObject(go.get());
	dae::TransformComponent::AddToGameObject(go.get());
	dae::TransformComponent::GetFromObject(go.get())->SetPosition(358, 180);
	dae::Texture2DComponent::AddToGameObject(go.get(), "logo.png");
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	dae::RenderComponent::AddToGameObject(go.get());
	dae::TransformComponent::AddToGameObject(go.get());
	dae::TransformComponent::GetFromObject(go.get())->SetPosition(292, 20);
	dae::TextComponent::AddToGameObject(go.get(), "Programming 4 Assignment", font, { 255, 255, 0, 255 });
	dae::TextComponent::GetFromObject(go.get())->SetColor({ 255, 0, 0, 255 });
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	dae::RenderComponent::AddToGameObject(go.get());
	dae::TransformComponent::AddToGameObject(go.get());
	dae::TransformComponent::GetFromObject(go.get())->SetPosition(0, 0);
	dae::TextComponent::AddToGameObject(go.get(), "60.0", font, { 255, 255, 255, 255 });
	dae::FpsComponent::AddToGameObject(go.get());
	scene.Add(std::move(go));
}

void Game::Update(float deltaTime)
{
	deltaTime;
}

void Game::Render() const
{

}