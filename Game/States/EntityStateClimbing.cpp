
#include "EntityStateClimbing.hpp"

#include "CustomCommands.hpp"
#include "EntityStateIdle.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "SpriteAnimation.hpp"
#include "Texture2DDisplay.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

EntityStateClimbing::EntityStateClimbing(Entity &entity, Direction dir)
    : EntityState(entity), m_CurrentMoveDir{dir},
      m_EntityTransform{entity.GetEntity()->GetComponent<dae::Transform>()} {
  OnEnter();
}

dae::EntityStateClimbing::~EntityStateClimbing() { OnExit(); }

std::unique_ptr<EntityState> dae::EntityStateClimbing::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto moveDir{input->GetMovementDirection()};
  auto level{m_Entity->GetLevel()};
  glm::vec2 charSize{32.f, 32.f};
  float deltaTime{TimeManager::GetInstance().GetDeltaTime()};
  auto worldPos{m_EntityTransform->GetWorldPosition()};

  if (moveDir.y == 0.f) {
    return std::make_unique<EntityStateIdle>(*m_Entity, m_CurrentMoveDir);
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
    return std::make_unique<EntityStateIdle>(*m_Entity, m_CurrentMoveDir);

  return nullptr;
}

void dae::EntityStateClimbing::Update() {
  m_EntityTransform->SetLocalPosition(
      m_EntityTransform->GetLocalPosition() +
      (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::EntityStateClimbing::OnEnter() {
  if (m_CurrentMoveDir == Direction::Up) {
    m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingUp");
    m_MovementVector = glm::vec2{0.f, -m_Entity->GetMoveSpeed()};
  } else {
    m_Entity->GetPlayerAnimation()->SetAnimationState("ClimbingDown");
    m_MovementVector = glm::vec2{0.f, m_Entity->GetMoveSpeed()};
  }
}

void dae::EntityStateClimbing::OnExit() { return; }
