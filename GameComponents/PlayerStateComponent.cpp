#include "PlayerStateComponent.h"
#include "GameObject.h"
#include "sdbm_hash.h"
using namespace dae;

dae::PlayerStateComponent::PlayerStateComponent(GameObject& owner)
	:Component(owner)
{
}

void dae::PlayerStateComponent::Notify(unsigned int eventId, GameObject*)
{
	//add functionality for checking how many starts and stops have been detected to see if we're still moving
	if (eventId == "OnPlayerStartedMoving"_h)
	{
		m_CurrentPlayerState = PlayerState::Running;
	}
	else if (eventId == "OnPlayerStoppedMoving"_h)
	{
		m_CurrentPlayerState = PlayerState::Idle;
	}
}

PlayerState dae::PlayerStateComponent::GetState() const
{
	return m_CurrentPlayerState;
}
