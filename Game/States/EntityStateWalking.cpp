#include "CustomCommands.hpp"
#include "EntityStateAttacking.hpp"
#include "EntityStateIdle.hpp"
#include "EntityStateWalking.hpp"
#include "GameObject.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "SpriteAnimation.hpp"
#include "TimeManager.hpp"
#include "Transform.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

dae::EntityStateWalking::EntityStateWalking(Entity &entity, Direction moveDir)
    : EntityState(entity),
      m_EntityTransform{entity.GetEntity()->GetComponent<Transform>()},
      m_CurrentMoveDir{moveDir} {
  OnEnter();
}

dae::EntityStateWalking::~EntityStateWalking() { OnExit(); }

std::unique_ptr<EntityState> dae::EntityStateWalking::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto moveDir{input->GetMovementDirection()};
  auto level{m_Entity->GetLevel()};
  glm::vec2 charSize{32.f, 32.f};
  float deltaTime{TimeManager::GetInstance().GetDeltaTime()};

  if (input->AttackButtonPressed())
    return std::make_unique<EntityStateAttacking>(*m_Entity, m_CurrentMoveDir);

  if (moveDir.x == 0.f) {
    return std::make_unique<EntityStateIdle>(*m_Entity, m_CurrentMoveDir);
  }
  // if we (in one frame) manage to switch direction
  // meaning in the same frame we release the old and press the new dir
  if (std::signbit(moveDir.x) != std::signbit(m_MovementVector.x)) {
    m_CurrentMoveDir =
        std::signbit(moveDir.x) ? Direction::Left : Direction::Right;
    if (m_CurrentMoveDir == Direction::Left) {
      m_Entity->GetEntityAnimation()->SetAnimationState("RunningLeft");
      m_MovementVector = glm::vec2{-m_Entity->GetMoveSpeed(), 0.f};
    } else if (m_CurrentMoveDir == Direction::Right) {
      m_Entity->GetEntityAnimation()->SetAnimationState("RunningRight");
      m_MovementVector = glm::vec2{m_Entity->GetMoveSpeed(), 0.f};
    }
  }

  if (!level->IsOnPlatform(m_EntityTransform->GetWorldPosition() +
                               m_MovementVector * deltaTime,
                           charSize))
    return std::make_unique<EntityStateIdle>(*m_Entity, m_CurrentMoveDir);

  return nullptr;
}

void dae::EntityStateWalking::Update() {
  m_EntityTransform->SetLocalPosition(
      m_EntityTransform->GetLocalPosition() +
      (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::EntityStateWalking::OnEnter() {
  if (m_CurrentMoveDir == Direction::Left) {
    m_Entity->GetEntityAnimation()->SetAnimationState("RunningLeft");
    m_MovementVector = glm::vec2{-m_Entity->GetMoveSpeed(), 0.f};
  } else if (m_CurrentMoveDir == Direction::Right) {
    m_Entity->GetEntityAnimation()->SetAnimationState("RunningRight");
    m_MovementVector = glm::vec2{m_Entity->GetMoveSpeed(), 0.f};
  }

  glm::vec2 charSize{32.f, 32.f};
  auto level{m_Entity->GetLevel()};

  // i know that in my game the players will never be attached to something so i
  // can take the local position
  auto pos = m_EntityTransform->GetLocalPosition();

  // this function take the bottom y coord and rounds to the height of the
  // closest platform that's why I first add and then subtract the char size
  pos.y = level->RoundToPlatformHeight(pos.y + charSize.y) - charSize.y;
  // set the new position
  m_EntityTransform->SetLocalPosition(pos);
}

void dae::EntityStateWalking::OnExit() {}
