#include "EntityStateClimbing.hpp"
#include "EntityStateIdle.hpp"
#include "EntityStateWalking.hpp"
#include "GameObject.hpp"
#include "InputInfo.hpp"
#include "InputManager.hpp"
#include "InputProvider.hpp"
#include "KeyboardInput.hpp"
#include "LevelGrid.hpp"
#include "SpriteAnimation.hpp"
#include "Texture2DDisplay.hpp"
#include "TimeManager.hpp"
#include "sdbm_hash.hpp"

using namespace dae;

EntityStateIdle::EntityStateIdle(Entity &entity, Direction direction)
    : EntityState(entity), m_CurrentFacingDir{direction} {
  OnEnter();
}

dae::EntityStateIdle::~EntityStateIdle() { OnExit(); }

std::unique_ptr<EntityState> dae::EntityStateIdle::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto movementDir{input->GetMovementDirection()};
  auto level{m_Entity->GetLevel()};
  auto worldPos{m_Entity->GetEntity()->GetWorldPosition()};
  glm::vec2 charSize{32.f, 32.f};
  auto deltaTime{TimeManager::GetInstance().GetDeltaTime()};
  auto moveSpeed{m_Entity->GetMoveSpeed()};

  if (movementDir.y < 0) {
    if (level->IsOnLadder(worldPos + glm::vec2{0.f, -moveSpeed * deltaTime},
                          charSize)) {
      return std::make_unique<EntityStateClimbing>(*m_Entity, Direction::Up);
    } else if (m_CurrentFacingDir != Direction::Up) {
      m_CurrentFacingDir = Direction::Up;
      m_Entity->GetPlayerAnimation()->SetAnimationState("IdleBack");
    }
  } else if (movementDir.y > 0) {
    // check both current and grid below the player
    if (level->IsOnLadder(worldPos + glm::vec2{0.f, moveSpeed * deltaTime},
                          charSize) ||
        level->IsOnLadder(worldPos + glm::vec2{0.f, 10.f}, charSize)) {
      return std::make_unique<EntityStateClimbing>(*m_Entity, Direction::Down);
    } else if (m_CurrentFacingDir != Direction::Down) {
      m_CurrentFacingDir = Direction::Down;
      m_Entity->GetPlayerAnimation()->SetAnimationState("IdleFront");
    }
  } else if (movementDir.x < 0) {
    if (level->IsOnPlatform(worldPos + glm::vec2{-moveSpeed * deltaTime, 0.f},
                            charSize)) {
      return std::make_unique<EntityStateWalking>(*m_Entity, Direction::Left);
    }
  } else if (movementDir.x > 0) {
    if (level->IsOnPlatform(worldPos + glm::vec2{moveSpeed * deltaTime, 0.f},
                            charSize)) {
      return std::make_unique<EntityStateWalking>(*m_Entity, Direction::Right);
    }
  }

  return nullptr;
}

void dae::EntityStateIdle::Update() { return; }

void dae::EntityStateIdle::OnEnter() {
  if (m_CurrentFacingDir == Direction::Down)
    m_Entity->GetPlayerAnimation()->SetAnimationState("IdleFront");
  else if (m_CurrentFacingDir == Direction::Up)
    m_Entity->GetPlayerAnimation()->SetAnimationState("IdleBack");
}

void dae::EntityStateIdle::OnExit() { return; }
