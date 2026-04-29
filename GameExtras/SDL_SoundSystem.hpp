#pragma once

#include "SoundSystem.hpp"
#include <memory>
namespace dae 
{
	class Impl;
	class SDL_SoundSystem : public SoundSystem
	{
	public:
		SDL_SoundSystem();
		~SDL_SoundSystem();
		void Play(sound_id const id, float const volume);
	private:

		std::unique_ptr<Impl> m_pImpl;
	};
}