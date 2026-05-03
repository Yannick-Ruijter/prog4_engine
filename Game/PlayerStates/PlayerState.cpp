#include "PlayerState.hpp"

using namespace dae;
dae::PlayerState::~PlayerState()
{
	OnExit();
}
std::unique_ptr<PlayerState> dae::PlayerState::HandleInput()
{
	return nullptr;
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

void dae::PlayerState::Notify(unsigned int , GameObject* )
{
}

dae::PlayerState::PlayerState(PlayerComponent& player)
	: m_Player{ &player } 
{
	OnEnter();
}
