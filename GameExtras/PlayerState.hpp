#pragma once
#include <memory>
#include "PlayerComponent.hpp"

namespace dae
{
	class PlayerComponent;
	class PlayerState
	{
	public:
		PlayerState(PlayerState const& other) = default;
		PlayerState(PlayerState&& other) = default;
		PlayerState& operator=(PlayerState const& other) = default;
		PlayerState& operator=(PlayerState&& other) = default;

		virtual ~PlayerState() = default;
		virtual std::unique_ptr<PlayerState> HandleInput();
		virtual void Update();
		virtual void OnEnter();
		virtual void OnExit();

	protected:
		PlayerState(PlayerComponent& player); 
		PlayerComponent* m_Player;
	};
}