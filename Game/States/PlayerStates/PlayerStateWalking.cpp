#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "PlayerStateIdle.hpp"
#include "PlayerStateWalking.hpp"
#include "SpriteAnimation.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::PlayerStateWalking::PlayerStateWalking(Entity &player, Direction moveDir)
    : EntityState(player),
      m_PlayerTransform{player.GetPlayer()->GetComponent<Transform>()},
      m_CurrentMoveDir{moveDir} {
  OnEnter();
}

dae::PlayerStateWalking::~PlayerStateWalking() { OnExit(); }

std::unique_ptr<EntityState> dae::PlayerStateWalking::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto moveDir{input->GetMovementDirection()};
  auto level{m_Entity->GetLevel()};
  glm::vec2 charSize{32.f, 32.f};
  float deltaTime{TimeManager::GetInstance().GetDeltaTime()};

  // if we (in one frame) manage to switch direction
  // meaning in the same frame we release the old and press the new dir
  if (std::signbit(moveDir.x) != std::signbit(m_MovementVector.x)) {
    m_CurrentMoveDir =
        std::signbit(moveDir.x) ? Direction::Left : Direction::Right;
    if (m_CurrentMoveDir == Direction::Left) {
      m_Entity->GetPlayerAnimation()->SetAnimationState("RunningLeft");
      m_MovementVector = glm::vec2{-m_Entity->GetMoveSpeed(), 0.f};
    } else if (m_CurrentMoveDir == Direction::Right) {
      m_Entity->GetPlayerAnimation()->SetAnimationState("RunningRight");
      m_MovementVector = glm::vec2{m_Entity->GetMoveSpeed(), 0.f};
    }
  }

  if (!level->IsOnPlatform(m_PlayerTransform->GetWorldPosition() +
                               m_MovementVector * deltaTime,
                           charSize))
    return std::make_unique<PlayerStateIdle>(*m_Entity);

  if (moveDir.x == 0.f) {
    return std::make_unique<PlayerStateIdle>(*m_Entity);
  }
  return nullptr;
}

void dae::PlayerStateWalking::Update() {
  m_PlayerTransform->SetLocalPosition(
      m_PlayerTransform->GetLocalPosition() +
      (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::PlayerStateWalking::OnEnter() {
  if (m_CurrentMoveDir == Direction::Left) {
    m_Entity->GetPlayerAnimation()->SetAnimationState("RunningLeft");
    m_MovementVector = glm::vec2{-m_Entity->GetMoveSpeed(), 0.f};
  } else if (m_CurrentMoveDir == Direction::Right) {
    m_Entity->GetPlayerAnimation()->SetAnimationState("RunningRight");
    m_MovementVector = glm::vec2{m_Entity->GetMoveSpeed(), 0.f};
  }

  glm::vec2 charSize{32.f, 32.f};
  auto level{m_Entity->GetLevel()};

  // i know that in my game the players will never be attached to something so i
  // can take the local position
  auto pos = m_PlayerTransform->GetLocalPosition();

  // this function take the bottom y coord and rounds to the height of the
  // closest platform that's why I first add and then subtract the char size
  pos.y = level->RoundToPlatformHeight(pos.y + charSize.y) - charSize.y;
  // set the new position
  m_PlayerTransform->SetLocalPosition(pos);
}

void dae::PlayerStateWalking::OnExit() {}
