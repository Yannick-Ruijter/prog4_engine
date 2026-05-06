
#include "PlayerStateClimbing.hpp"

#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateIdle.hpp"
#include "Texture2DComponent.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

PlayerStateClimbing::PlayerStateClimbing(PlayerComponent &player, MoveDirection dir)
    : PlayerState(player), m_CurrentMoveDir{dir}
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
        return std::make_unique<PlayerStateIdle>(*m_Player, m_CurrentMoveDir);
    return nullptr;
}

void dae::PlayerStateClimbing::Update()
{
    return;
}

void dae::PlayerStateClimbing::OnEnter()
{
    if (m_CurrentMoveDir == MoveDirection::Up)
        m_Player->GetPlayerAnimation()->SetAnimationState("ClimbingUp");
    else
        m_Player->GetPlayerAnimation()->SetAnimationState("ClimbingDown");
}

void dae::PlayerStateClimbing::OnExit()
{
    return;
}

void dae::PlayerStateClimbing::Notify(unsigned int eventId, GameObject *)
{
    if (eventId == "OnMoveUpButtonReleased"_h)
    {
        if (m_CurrentMoveDir == MoveDirection::Up)
            m_PlayerStoppedClimbing = true;
    }
    else if (eventId == "OnMoveDownButtonReleased"_h)
    {
        if (m_CurrentMoveDir == MoveDirection::Down)
            m_PlayerStoppedClimbing = true;
    }
}
