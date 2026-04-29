#define WIN32_LEAN_AND_MEAN
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.hpp"
#include "Command.hpp"

using namespace dae;
bool dae::InputManager::ProcessInput()
{
	for (int i{ 0 }; i < 4; ++i)
	{
		m_ControllerInput[i].ProcessInput();
	}
	m_KeyboardInput.ProcessInput();
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {

		}
		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

		}
		// etc...
		ImGui_ImplSDL3_ProcessEvent(&e);
	}
	
	return true;
}

ControllerInput* dae::InputManager::GetControllerInput(int controllerIndex)
{
	assert(controllerIndex < 4 && "only up to 4 controllers are supported, controllerIndex can not be bigger than 3");
	return &m_ControllerInput[controllerIndex];
}

KeyboardInput* dae::InputManager::GetKeyboardInput()
{
	return &m_KeyboardInput;
}
