#include "PlayerState.hpp"

using namespace dae;
std::unique_ptr<PlayerState> dae::PlayerState::HandleInput()
{
	return std::unique_ptr<PlayerState>();
}

void dae::PlayerState::Update()
{
}

void dae::PlayerState::OnEnter()
{
}

void dae::PlayerState::OnExit()
{
}

dae::PlayerState::PlayerState(PlayerComponent& player)
	: m_Player{ &player } 
{
}
