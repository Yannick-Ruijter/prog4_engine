
#include "PlayerStateClimbing.hpp"

#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateIdle.hpp"
#include "Texture2DComponent.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

PlayerStateClimbing::PlayerStateClimbing(PlayerComponent &player) : PlayerState(player)
{
    OnEnter();
}

dae::PlayerStateClimbing::~PlayerStateClimbing()
{
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateClimbing::HandleInput()
{
    if (m_PlayerStoppedClimbing)
        return std::make_unique<PlayerStateIdle>(*m_Player);
    return nullptr;
}

void dae::PlayerStateClimbing::Update()
{
    return;
}

void dae::PlayerStateClimbing::OnEnter()
{
    m_Player->GetPlayerAnimation()->SetAnimationState("IdleFront");
}

void dae::PlayerStateClimbing::OnExit()
{
    return;
}

void dae::PlayerStateClimbing::Notify(unsigned int eventId, GameObject *)
{
    if (eventId == "OnPlayerStoppedClimbing"_h)
    {
        m_PlayerStoppedClimbing = true;
    }
}
