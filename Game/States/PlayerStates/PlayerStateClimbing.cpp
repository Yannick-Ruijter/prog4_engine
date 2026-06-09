
#include "PlayerStateClimbing.hpp"

#include "CustomCommands.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "PlayerStateIdle.hpp"
#include "SpriteAnimation.hpp"
#include "Texture2DDisplay.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

PlayerStateClimbing::PlayerStateClimbing(Entity &player, Direction dir)
    : EntityState(player), m_CurrentMoveDir{dir},
      m_PlayerTransform{player.GetPlayer()->GetComponent<dae::Transform>()} {
  OnEnter();
}

dae::PlayerStateClimbing::~PlayerStateClimbing() { OnExit(); }

std::unique_ptr<EntityState> dae::PlayerStateClimbing::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto moveDir{input->GetMovementDirection()};
  auto level{m_Entity->GetLevel()};
  glm::vec2 charSize{32.f, 32.f};
  float deltaTime{TimeManager::GetInstance().GetDeltaTime()};
  auto worldPos{m_PlayerTransform->GetWorldPosition()};

  if (moveDir.y == 0.f) {
    return std::make_unique<PlayerStateIdle>(*m_Entity, m_CurrentMoveDir);
  }

  if (std::signbit(moveDir.y) != std::signbit(m_MovementVector.y)) {
    m_CurrentMoveDir =
        std::signbit(moveDir.x) ? Direction::Up : Direction::Down;
    if (m_CurrentMoveDir == Direction::Up) {
      m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingUp");
      m_MovementVector = glm::vec2{0.f, -m_Entity->GetMoveSpeed()};
    } else {
      m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingDown");
      m_MovementVector = glm::vec2{0.f, m_Entity->GetMoveSpeed()};
    }
  }

  // check both current and grid below it
  if (!level->IsOnLadder(worldPos + m_MovementVector * deltaTime, charSize) &&
      !level->IsOnLadder(worldPos + glm::vec2{0.f, 10.f}, charSize))
    return std::make_unique<PlayerStateIdle>(*m_Entity, m_CurrentMoveDir);

  return nullptr;
}

void dae::PlayerStateClimbing::Update() {
  m_PlayerTransform->SetLocalPosition(
      m_PlayerTransform->GetLocalPosition() +
      (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::PlayerStateClimbing::OnEnter() {
  if (m_CurrentMoveDir == Direction::Up) {
    m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingUp");
    m_MovementVector = glm::vec2{0.f, -m_Entity->GetMoveSpeed()};
  } else {
    m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingDown");
    m_MovementVector = glm::vec2{0.f, m_Entity->GetMoveSpeed()};
  }
}

void dae::PlayerStateClimbing::OnExit() { return; }
