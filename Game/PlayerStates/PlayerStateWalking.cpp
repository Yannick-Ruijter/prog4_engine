#include "PlayerStateWalking.hpp"
#include "CustomCommands.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateIdle.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::PlayerStateWalking::PlayerStateWalking(PlayerComponent &player, MoveDirection moveDir)
    : PlayerState(player), m_CurrentMoveDir{moveDir}
{
    OnEnter();
}

dae::PlayerStateWalking::~PlayerStateWalking()
{
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateWalking::HandleInput()
{
    if (m_StoppedMoving)
        return std::make_unique<PlayerStateIdle>(*m_Player);
    return nullptr;
}

void dae::PlayerStateWalking::Update()
{
}

void dae::PlayerStateWalking::OnEnter()
{
    if (m_CurrentMoveDir == MoveDirection::Left)
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningLeft");
    else if (m_CurrentMoveDir == MoveDirection::Right)
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningRight");
}

void dae::PlayerStateWalking::OnExit()
{
}

void dae::PlayerStateWalking::Notify(unsigned int eventId, GameObject *)
{
    // still need a better way to link commands and states but this works for now
    if (eventId == "OnMoveLeftButtonReleased"_h)
    {
        if (m_CurrentMoveDir == MoveDirection::Left)
            m_StoppedMoving = true;
    }
    else if (eventId == "OnMoveRightButtonReleased"_h)
    {
        if (m_CurrentMoveDir == MoveDirection::Right)
            m_StoppedMoving = true;
    }
}
