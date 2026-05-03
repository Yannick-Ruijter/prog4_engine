#pragma once

#include "Component.hpp"
#include <memory>
namespace dae
{
	class PlayerState;
	class PlayerComponent final: public Component
	{
	public:
		PlayerComponent(GameObject& owner, std::unique_ptr<PlayerState>& startingState);
		~PlayerComponent();
		void Update();
	private:
		std::unique_ptr<PlayerState> m_CurrentState;
	};
}