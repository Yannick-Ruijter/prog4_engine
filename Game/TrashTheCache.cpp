#include "TrashTheCache.h"
#include "FPSComponent.h"
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Texture2DComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

void InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	SDL_Window* window = dae::Renderer::GetInstance().GetWindow();
	SDL_Renderer* renderer = dae::Renderer::GetInstance().GetSDLRenderer();
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
}

TrashTheCache::~TrashTheCache()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void TrashTheCache::Initialize()
{
	InitializeImGui();

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
}

void TrashTheCache::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ShowEx1Screen();
	ShowEx2Screen();

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(
		ImGui::GetDrawData(),
		dae::Renderer::GetInstance().GetSDLRenderer()
	);
}

void TrashTheCache::ShowEx1Screen() const
{
	static int samples = 100;
	
	ImGui::Begin("Exercise 1");

	ImGui::SetNextItemWidth(80.0f);
	ImGui::InputInt("##samples", &samples);

	ImGui::SameLine();
	ImGui::Text("# samples");

	ImGui::Spacing();

	ImGui::Button("Trash the cache");

	ImGui::End();
}

void TrashTheCache::ShowEx2Screen() const
{
	static int samples = 100;

	ImGui::SetNextWindowSize(ImVec2(420.0f, 0.0f));
	ImGui::Begin("Exercise 2");

	ImGui::SetNextItemWidth(180.0f);
	ImGui::InputInt("##samples", &samples);

	ImGui::SameLine();
	ImGui::Text("# samples");

	ImGui::Spacing();

	ImGui::Button("Trash the cache with GameObject3D");
	ImGui::Button("Trash the cache with GameObject3DAlt");

	ImGui::End();
}
