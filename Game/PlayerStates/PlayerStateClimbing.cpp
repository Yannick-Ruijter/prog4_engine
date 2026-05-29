
#include "PlayerStateClimbing.hpp"

#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "KeyboardInput.hpp"
#include "LevelGridComponent.hpp"
#include "PlayerAnimationComponent.hpp"
#include "PlayerStateIdle.hpp"
#include "Texture2DComponent.hpp"
#include "TimeManager.hpp"
#include "TransformComponent.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

PlayerStateClimbing::PlayerStateClimbing(PlayerComponent &player, Direction dir)
    : PlayerState(player),
      m_CurrentMoveDir{dir},
      m_PlayerTransform{player.GetPlayer()->GetComponent<dae::TransformComponent>()}
{
    OnEnter();
}

dae::PlayerStateClimbing::~PlayerStateClimbing()
{
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateClimbing::HandleInput()
{
    auto input{m_Player->GetInput()};
    auto level{m_Player->GetLevel()};
    glm::vec2 charSize{32.f, 32.f};
    float deltaTime{TimeManager::GetInstance().GetDeltaTime()};
    auto worldPos{m_PlayerTransform->GetWorldPosition()};

    // check both current and grid below it
    if (!level->IsOnLadder(worldPos + m_MovementVector * deltaTime, charSize) &&
        !level->IsOnLadder(worldPos + glm::vec2{0.f, 10.f}, charSize))
        return std::make_unique<PlayerStateIdle>(*m_Player, m_CurrentMoveDir);

    if (m_CurrentMoveDir == Direction::Up)
    {
        if (input->WasReleasedThisFrame(InputAction::MoveUp))
            return std::make_unique<PlayerStateIdle>(*m_Player, m_CurrentMoveDir);
    }
    else
    {
        if (input->WasReleasedThisFrame(InputAction::MoveDown))
            return std::make_unique<PlayerStateIdle>(*m_Player, m_CurrentMoveDir);
    }
    return nullptr;
}

void dae::PlayerStateClimbing::Update()
{
    m_PlayerTransform->SetLocalPosition(
        m_PlayerTransform->GetLocalPosition() + (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::PlayerStateClimbing::OnEnter()
{
    if (m_CurrentMoveDir == Direction::Up)
    {
        m_Player->GetPlayerAnimation()->SetAnimationState("ClimbingUp");
        m_MovementVector = glm::vec2{0.f, -m_Player->GetMoveSpeed()};
    }
    else
    {
        m_Player->GetPlayerAnimation()->SetAnimationState("ClimbingDown");
        m_MovementVector = glm::vec2{0.f, m_Player->GetMoveSpeed()};
    }
}

void dae::PlayerStateClimbing::OnExit()
{
    return;
}
