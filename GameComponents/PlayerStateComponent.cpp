#include "PlayerStateComponent.hpp"
#include "GameObject.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

dae::PlayerStateComponent::PlayerStateComponent(GameObject& owner)
	:Component(owner)
{
}

void dae::PlayerStateComponent::Notify(unsigned int eventId, GameObject*)
{
	if (eventId == "OnPlayerStartedMoving"_h)
	{
		++m_NrCurrentWalkCommands;
		m_CurrentPlayerState = PlayerState::Running;
	}
	else if (eventId == "OnPlayerStoppedMoving"_h)
	{
		--m_NrCurrentWalkCommands;
		if(m_NrCurrentWalkCommands <= 0) m_CurrentPlayerState = PlayerState::Idle;
	}
}

PlayerState dae::PlayerStateComponent::GetState() const
{
	return m_CurrentPlayerState;
}
