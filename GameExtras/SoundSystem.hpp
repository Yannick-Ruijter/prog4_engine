#pragma once

#include "Service.h"
using sound_id = unsigned short;

namespace dae
{
	class SoundSystem : public service {
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(sound_id const id, float const volume) = 0;
	};
}