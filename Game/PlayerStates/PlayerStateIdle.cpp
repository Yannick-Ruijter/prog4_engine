#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "LevelGridComponent.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateClimbing.hpp"
#include "PlayerStateIdle.hpp"
#include "PlayerStateWalking.hpp"
#include "Texture2DComponent.hpp"
#include "TimeManager.hpp"
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
    auto deltaTime{TimeManager::GetInstance().GetDeltaTime()};
    auto moveSpeed{m_Player->GetMoveSpeed()};

    if (input->WasPressedThisFrame(InputAction::MoveUp))
    {
        if (level->IsOnLadder(worldPos + glm::vec2{0.f, -moveSpeed * deltaTime}, charSize))
        {
            return std::make_unique<PlayerStateClimbing>(*m_Player, MoveDirection::Up);
        }
        else if (m_CurrentFacingDir != MoveDirection::Up)
        {
            m_CurrentFacingDir = MoveDirection::Up;
            m_Player->GetPlayerAnimation()->SetAnimationState("IdleBack");
        }
    }
    else if (input->WasPressedThisFrame(InputAction::MoveDown))
    {
        // check both current and grid below the player
        if (level->IsOnLadder(worldPos + glm::vec2{0.f, moveSpeed * deltaTime}, charSize) ||
            level->IsOnLadder(worldPos + glm::vec2{0.f, 10.f}, charSize))
        {
            return std::make_unique<PlayerStateClimbing>(*m_Player, MoveDirection::Down);
        }
        else if (m_CurrentFacingDir != MoveDirection::Down)
        {
            m_CurrentFacingDir = MoveDirection::Down;
            m_Player->GetPlayerAnimation()->SetAnimationState("IdleFront");
        }
    }
    else if (input->WasPressedThisFrame(InputAction::MoveLeft))
    {
        if (level->IsOnPlatform(worldPos + glm::vec2{-moveSpeed * deltaTime, 0.f}, charSize))
        {
            return std::make_unique<PlayerStateWalking>(*m_Player, MoveDirection::Left);
        }
    }
    else if (input->WasPressedThisFrame(InputAction::MoveRight))
    {
        if (level->IsOnPlatform(worldPos + glm::vec2{moveSpeed * deltaTime, 0.f}, charSize))
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
