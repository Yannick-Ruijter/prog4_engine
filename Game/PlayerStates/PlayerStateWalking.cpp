#include "PlayerStateWalking.hpp"
#include "PlayerStateIdle.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::PlayerStateWalking::PlayerStateWalking(PlayerComponent& player)
	:PlayerState(player)
{
	OnEnter();
}

dae::PlayerStateWalking::~PlayerStateWalking()
{
	OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateWalking::HandleInput()
{
	if (m_StoppedMoving) return std::make_unique<PlayerStateIdle>(*m_Player);
	return nullptr;
}

void dae::PlayerStateWalking::Update()
{
}

void dae::PlayerStateWalking::OnEnter()
{
}

void dae::PlayerStateWalking::OnExit()
{
}

void dae::PlayerStateWalking::Notify(unsigned int eventId, GameObject*)
{
	if (eventId == "OnPlayerStoppedMoving"_h) m_StoppedMoving = true;
}
