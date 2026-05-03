#include "PlayerComponent.hpp"
#include "GameObject.hpp"
#include "PlayerState.hpp"
using namespace dae;

PlayerComponent::PlayerComponent(GameObject& owner, std::unique_ptr<PlayerState>& startingState)
	:Component(owner), m_CurrentState{ std::move(startingState)}
{
	
}

dae::PlayerComponent::~PlayerComponent()
{
}

void dae::PlayerComponent::Update()
{
	auto tempState = m_CurrentState->HandleInput();
	if (tempState != nullptr) m_CurrentState = std::move(tempState);
	m_CurrentState->Update();
}
