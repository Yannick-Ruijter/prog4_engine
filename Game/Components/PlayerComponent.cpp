#include "PlayerComponent.hpp"
#include "GameObject.hpp"
#include "PlayerState.hpp"
#include "PlayerStateIdle.hpp"
using namespace dae;

PlayerComponent::PlayerComponent(GameObject& owner)
	:Component(owner), m_CurrentState{ std::make_unique<PlayerStateIdle>(*this)}
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

GameObject* dae::PlayerComponent::GetPlayer()
{
	return GetOwner();
}

void dae::PlayerComponent::Notify(unsigned int eventId, GameObject* source)
{
	if (source != GetOwner()) return;
	m_CurrentState->Notify(eventId, source);
}
