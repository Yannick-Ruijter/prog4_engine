#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
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

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
