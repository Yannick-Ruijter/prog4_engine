#pragma once

#include "SoundEventQueue.hpp"
#include "SoundSystem.hpp"
#include <memory>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
namespace dae
{
    class SDL_SoundSystem : public SoundSystem
    {
      public:
        SDL_SoundSystem(std::vector<std::pair<sound_id, std::string>> soundsToPreload);
        ~SDL_SoundSystem();
        void Play(sound_id const id, float const volume);
        void Destroy();

      private:
        bool m_HasBeenDestroyed{false};
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
} // namespace dae
