#include "PlayerStateIdle.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "LevelGridComponent.hpp"
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
    auto input{m_Player->GetInput()};
    auto level{m_Player->GetLevel()};
    auto worldPos{m_Player->GetPlayer()->GetWorldPosition()};
    glm::vec2 charSize{32.f, 32.f};
    if (input->WasPressedThisFrame(InputAction::MoveUp))
    {
        if (level->IsOnLadder(worldPos, charSize))
        {
            return std::make_unique<PlayerStateClimbing>(*m_Player, MoveDirection::Up);
        }
    }

    if (input->WasPressedThisFrame(InputAction::MoveDown))
    {
        if (level->IsOnLadder(worldPos, charSize))
        {
            return std::make_unique<PlayerStateClimbing>(*m_Player, MoveDirection::Down);
        }
    }

    if (input->WasPressedThisFrame(InputAction::MoveLeft))
    {
        if (level->IsOnPlatform(worldPos, charSize))
        {
            return std::make_unique<PlayerStateWalking>(*m_Player, MoveDirection::Left);
        }
    }

    if (input->WasPressedThisFrame(InputAction::MoveRight))
    {
        if (level->IsOnPlatform(worldPos, charSize))
        {
            return std::make_unique<PlayerStateWalking>(*m_Player, MoveDirection::Right);
        }
    }

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
