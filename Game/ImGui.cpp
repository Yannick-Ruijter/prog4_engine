#include "ImGui.h"
#include "FPSComponent.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "OrbitComponent.h"

void ImGui::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

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
	dae::GameObject* temp{ go.get() };
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 200, 200, 0 });
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::Texture2DComponent>("Data/RotatingAsset.png");
	go->AddComponent<dae::OrbitComponent>(30.f, 2.f);
	go->SetParent(temp, false);
	temp = go.release();

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::Texture2DComponent>("Data/RotatingAsset.png");
	go->AddComponent<dae::OrbitComponent>(30.f, 5.f);
	go->SetParent(temp, false);
	temp = go.release();
}