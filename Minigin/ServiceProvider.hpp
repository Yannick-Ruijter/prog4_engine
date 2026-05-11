#pragma once

#include "SDL_SoundSystem.hpp"
#include "Service.hpp"
#include "Singleton.hpp"
#include "SoundSystem.hpp"
#include <memory>
#include <vector>

namespace dae
{
    class NullSoundSystem final : public SoundSystem
    {
      public:
        NullSoundSystem() = default;
        virtual ~NullSoundSystem() override = default;
        void Play(const sound_id, const float) override
        {
        }
        void Destroy() override
        {
        }
    };
    class ServiceProvider final
    {
        static std::unique_ptr<SoundSystem> _ss_instance;

      public:
        static SoundSystem &GetSoundSystem()
        {
            return *_ss_instance;
        }
        static void RegisterSoundSystem(std::unique_ptr<SoundSystem> &&ss)
        {
            _ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
        }
    };
} // namespace dae

inline std::unique_ptr<dae::SoundSystem> dae::ServiceProvider::_ss_instance = std::make_unique<NullSoundSystem>();
