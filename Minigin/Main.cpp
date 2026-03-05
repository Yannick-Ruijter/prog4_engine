#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//TODO: Leak detector is not working, CMAKE ,, find_package(VLD CONFIG) not detecting vld"
#if _DEBUG
#if __has_include(<vld.h>)
#pragma message(">>> VLD HEADER FOUND - including vld.h")
#include <vld.h>
#else
#pragma message(">>> VLD HEADER NOT FOUND")
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "BurgerTime.h"
#include "TrashTheCache.h"

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
