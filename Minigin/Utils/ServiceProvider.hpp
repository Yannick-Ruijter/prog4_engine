#pragma once

#include "SDL_SoundSystem.hpp"
#include "Singleton.hpp"
#include "SoundSystem.hpp"
#include <memory>
#include <random>
#include <vector>

namespace dae {
class NullSoundSystem final : public SoundSystem {
public:
  NullSoundSystem() = default;
  virtual ~NullSoundSystem() override = default;
  void Play(const sound_id, const float) override {}
  void Destroy() override {}
  void ToggleVolume() override {}
};

class RandomProvider {
private:
  std::mt19937 m_Rng;

public:
  RandomProvider() : m_Rng{std::random_device{}()} {}
  std::mt19937 &GetRng() { return m_Rng; }
};

class ServiceProvider final {
  static std::unique_ptr<SoundSystem> _ss_instance;
  static std::unique_ptr<RandomProvider> _random_instance;

public:
  static SoundSystem &GetSoundSystem() { return *_ss_instance; }
  static RandomProvider &GetRandomProvider() { return *_random_instance; }
  static void RegisterSoundSystem(std::unique_ptr<SoundSystem> &&ss) {
    _ss_instance =
        ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
  }
};
} // namespace dae

inline std::unique_ptr<dae::SoundSystem> dae::ServiceProvider::_ss_instance =
    std::make_unique<NullSoundSystem>();
inline std::unique_ptr<dae::RandomProvider>
    dae::ServiceProvider::_random_instance = std::make_unique<RandomProvider>();
