#include "PlayerStateIdle.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateClimbing.hpp"
#include "PlayerStateWalking.hpp"
#include "Texture2DComponent.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

PlayerStateIdle::PlayerStateIdle(PlayerComponent &player, MoveDirection direction)
    : PlayerState(player), m_CurrentFacingDir{direction}
{
    OnEnter();
}

dae::PlayerStateIdle::~PlayerStateIdle()
{
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateIdle::HandleInput()
{
    if (m_StartedMoving)
        return std::make_unique<PlayerStateWalking>(*m_Player, m_CurrentFacingDir);
    else if (m_StartedClimbing)
        return std::make_unique<PlayerStateClimbing>(*m_Player, m_CurrentFacingDir);
    return nullptr;
}

void dae::PlayerStateIdle::Update()
{
    return;
}

void dae::PlayerStateIdle::OnEnter()
{
    if (m_CurrentFacingDir == MoveDirection::Down)
        m_Player->GetPlayerAnimation()->SetAnimationState("IdleFront");
    else if (m_CurrentFacingDir == MoveDirection::Up)
        m_Player->GetPlayerAnimation()->SetAnimationState("IdleBack");
}

void dae::PlayerStateIdle::OnExit()
{
    return;
}

void dae::PlayerStateIdle::Notify(unsigned int eventId, GameObject *)
{
    if (eventId == "OnMoveLeftButtonPressed"_h)
    {
        m_StartedMoving = true;
        m_CurrentFacingDir = MoveDirection::Left;
    }
    else if (eventId == "OnMoveRightButtonPressed"_h)
    {
        m_StartedMoving = true;
        m_CurrentFacingDir = MoveDirection::Right;
    }
    else if (eventId == "OnMoveUpButtonPressed"_h)
    {
        m_StartedClimbing = true;
        m_CurrentFacingDir = MoveDirection::Up;
    }
    else if (eventId == "OnMoveDownButtonPressed"_h)
    {
        m_StartedClimbing = true;
        m_CurrentFacingDir = MoveDirection::Down;
    }
}
