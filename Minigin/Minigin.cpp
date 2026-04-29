#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#undef USE_STEAMWORKS
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif


#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <chrono>

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath, std::unique_ptr<Game> game)
	:m_Game{std::move(game)}
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"BurgerTime",
		1032,
		800,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

#if USE_STEAMWORKS
	if (!SteamAPI_Init())
		throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
#endif

	Renderer::GetInstance().Init(g_window, m_Game.get());
	ResourceManager::GetInstance().Init(dataPath);
	m_Game->Initialize();
}

dae::Minigin::~Minigin()
{
#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void dae::Minigin::RunOneFrame()
{
#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif 
	TimeManager::GetInstance().Update();
	auto startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	m_quit = !InputManager::GetInstance().ProcessInput();
	m_Game->Update();
	SceneManager::GetInstance().Update();
	SceneManager::GetInstance().LateUpdate();
	Renderer::GetInstance().Render();
	auto currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	auto diff = currentTime - startTime;
	//force fps to be 60-ish
	while (diff < 16666666)
	{
		currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		diff = currentTime - startTime;
	}
}
