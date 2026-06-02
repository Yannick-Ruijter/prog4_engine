#pragma once
using sound_id = unsigned short;

namespace dae {
    class SoundSystem {
      public:
        virtual ~SoundSystem() = default;
        virtual void Play(sound_id const id, float const volume) = 0;
        virtual void Destroy() = 0;
        virtual void ToggleVolume() = 0;
    };
} // namespace dae
