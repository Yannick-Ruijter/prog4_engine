#include "PlayerStateIdle.hpp"
#include "PlayerStateWalking.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "InputInfo.hpp"
#include "sdbm_hash.hpp"
#include "GameObject.hpp"
#include "Texture2DComponent.hpp"
using namespace dae;

PlayerStateIdle::PlayerStateIdle(PlayerComponent& player)
	:PlayerState(player)
{

}

dae::PlayerStateIdle::~PlayerStateIdle()
{
	OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateIdle::HandleInput()
{
	if (m_StartedMoving) return std::make_unique<PlayerStateWalking>(*m_Player);
	return nullptr;
}

void dae::PlayerStateIdle::Update()
{
	return;
}

void dae::PlayerStateIdle::OnEnter()
{
	return;
}

void dae::PlayerStateIdle::OnExit()
{
	return;
}

void dae::PlayerStateIdle::Notify(unsigned int eventId, GameObject*)
{
	if (eventId == "OnPlayerStartedMoving"_h)
	{
		m_StartedMoving = true;
	}
}
