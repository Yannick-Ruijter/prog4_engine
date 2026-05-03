#pragma once
#include <memory>
#include "PlayerComponent.hpp"
#include "Observer.hpp"
namespace dae
{
	class PlayerComponent;
	class PlayerState : public Observer
	{
	public:
		PlayerState(PlayerState const& other) = default;
		PlayerState(PlayerState&& other) = default;
		PlayerState& operator=(PlayerState const& other) = default;
		PlayerState& operator=(PlayerState&& other) = default;

		virtual ~PlayerState();
		virtual std::unique_ptr<PlayerState> HandleInput();
		virtual void Update();
		virtual void OnEnter();
		virtual void OnExit();
		virtual void Notify(unsigned int eventId, GameObject* source);
	protected:
		PlayerState(PlayerComponent& player); 
		PlayerComponent* m_Player;
	};
}