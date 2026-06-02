#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "LevelGrid.hpp"
#include "PlayerAnimation.hpp"
#include "PlayerInput.hpp"
#include "PlayerStateIdle.hpp"
#include "PlayerStateWalking.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::PlayerStateWalking::PlayerStateWalking(PlayerController &player, Direction moveDir)
    : PlayerState(player),
      m_PlayerTransform{player.GetPlayer()->GetComponent<Transform>()},
      m_CurrentMoveDir{moveDir} {
    OnEnter();
}

dae::PlayerStateWalking::~PlayerStateWalking() {
    OnExit();
}

std::unique_ptr<PlayerState> dae::PlayerStateWalking::HandleInput() {
    auto input{m_Player->GetInput()};
    auto level{m_Player->GetLevel()};
    glm::vec2 charSize{32.f, 32.f};
    float deltaTime{TimeManager::GetInstance().GetDeltaTime()};

    if (!level->IsOnPlatform(m_PlayerTransform->GetWorldPosition() + m_MovementVector * deltaTime, charSize))
        return std::make_unique<PlayerStateIdle>(*m_Player);

    if (m_CurrentMoveDir == Direction::Left) {
        if (input->WasReleasedThisFrame(InputAction::MoveLeft))
            return std::make_unique<PlayerStateIdle>(*m_Player);
    } else {
        if (input->WasReleasedThisFrame(InputAction::MoveRight))
            return std::make_unique<PlayerStateIdle>(*m_Player);
    }
    return nullptr;
}

void dae::PlayerStateWalking::Update() {
    m_PlayerTransform->SetLocalPosition(
        m_PlayerTransform->GetLocalPosition() + (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::PlayerStateWalking::OnEnter() {
    if (m_CurrentMoveDir == Direction::Left) {
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningLeft");
        m_MovementVector = glm::vec2{-m_Player->GetMoveSpeed(), 0.f};
    } else if (m_CurrentMoveDir == Direction::Right) {
        m_Player->GetPlayerAnimation()->SetAnimationState("RunningRight");
        m_MovementVector = glm::vec2{m_Player->GetMoveSpeed(), 0.f};
    }

    glm::vec2 charSize{32.f, 32.f};
    auto level{m_Player->GetLevel()};

    // i know that in my game the players will never be attached to something so i can take the local position
    auto pos = m_PlayerTransform->GetLocalPosition();

    // this function take the bottom y coord and rounds to the height of the closest platform
    // that's why I first add and then subtract the char size
    pos.y = level->RoundToPlatformHeight(pos.y + charSize.y) - charSize.y;
    // set the new position
    m_PlayerTransform->SetLocalPosition(pos);
}

void dae::PlayerStateWalking::OnExit() {
}
