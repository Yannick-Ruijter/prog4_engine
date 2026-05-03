#pragma once

#include "Component.hpp"
#include "PlayerState.hpp"

namespace dae
{
	class PlayerComponent : public Component
	{
	public:
		PlayerComponent(GameObject& owner);
	};
}