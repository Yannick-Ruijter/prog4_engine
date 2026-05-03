#pragma once

#include "SoundSystem.hpp"
#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include "SoundEventQueue.hpp"
namespace dae 
{
	class Impl;
	class SDL_SoundSystem : public SoundSystem
	{
	public:
		SDL_SoundSystem(std::vector<std::pair<sound_id, std::string>> soundsToPreload);
		~SDL_SoundSystem();
		void Play(sound_id const id, float const volume);
		void Destroy();
	private:
		bool m_HasBeenDestroyed{ false };
		std::unique_ptr<Impl> m_pImpl;
	};
}