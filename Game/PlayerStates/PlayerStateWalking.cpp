#include "PlayerStateWalking.hpp"
#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "LevelGridComponent.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerInput.hpp"
#include "PlayerStateIdle.hpp"
#include "TimeManager.hpp"
#include "TransformComponent.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::PlayerStateWalking::PlayerStateWalking(PlayerComponent &player, MoveDirection moveDir)
    : PlayerState(player),
      m_PlayerTransform{player.GetPlayer()->GetComponent<TransformComponent>()},
      m_CurrentMoveDir{moveDir}
{
    OnEnter();
}

dae::PlayerStateWalking::~PlayerStateWalking()
{
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateWalking::HandleInput()
{
    auto input{m_Player->GetInput()};
    auto level{m_Player->GetLevel()};
    glm::vec2 charSize{32.f, 32.f};
    float deltaTime{TimeManager::GetInstance().GetDeltaTime()};

    if (!level->IsOnPlatform(m_PlayerTransform->GetWorldPosition() + m_MovementVector * deltaTime, charSize))
        return std::make_unique<PlayerStateIdle>(*m_Player);

    if (m_CurrentMoveDir == MoveDirection::Left)
    {
        if (input->WasReleasedThisFrame(InputAction::MoveLeft))
            return std::make_unique<PlayerStateIdle>(*m_Player);
    }
    else
    {
        if (input->WasReleasedThisFrame(InputAction::MoveRight))
            return std::make_unique<PlayerStateIdle>(*m_Player);
    }
    return nullptr;
}

void dae::PlayerStateWalking::Update()
{
    m_PlayerTransform->SetLocalPosition(
        m_PlayerTransform->GetLocalPosition() + (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::PlayerStateWalking::OnEnter()
{
    if (m_CurrentMoveDir == MoveDirection::Left)
    {
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningLeft");
        m_MovementVector = glm::vec2{-m_Player->GetMoveSpeed(), 0.f};
    }
    else if (m_CurrentMoveDir == MoveDirection::Right)
    {
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningRight");
        m_MovementVector = glm::vec2{m_Player->GetMoveSpeed(), 0.f};
    }
}

void dae::PlayerStateWalking::OnExit()
{
}
