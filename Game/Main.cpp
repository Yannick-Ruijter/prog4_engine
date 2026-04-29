#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG
#if __has_include(<vld.h>)
#pragma message(">>> VLD HEADER FOUND - including vld.h")
#include <vld.h>
#else
#pragma message(">>> VLD HEADER NOT FOUND")
#endif
#endif

#include "Minigin.hpp"
#include "SceneManager.hpp"
#include "ResourceManager.hpp"
#include "RenderComponent.hpp"
#include "TextComponent.hpp"
#include "Scene.hpp"
#include "BurgerTime.hpp"

#include <filesystem>
namespace fs = std::filesystem;


int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location, std::make_unique<BurgerTime>());
	engine.Run();
    return 0;
}
