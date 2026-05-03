#pragma once

#include "Service.hpp"
using sound_id = unsigned short;

namespace dae
{
	class SoundSystem : public Service {
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(sound_id const id, float const volume) = 0;
	};
}