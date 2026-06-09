#include "CustomCommands.hpp"
#include "Entity.hpp"
#include "EntityStateAttacking.hpp"
#include "EntityStateClimbing.hpp"
#include "EntityStateIdle.hpp"
#include "EntityStateWalking.hpp"
#include "GameObject.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "ObjectRenderer.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "SpriteAnimation.hpp"
#include "TimeManager.hpp"

using namespace dae;

dae::EntityStateAttacking::EntityStateAttacking(Entity &entity, Direction dir)
    : EntityState(entity), m_CurrentFaceDir{dir},
      m_EntityTransform{entity.GetEntity()->GetComponent<dae::Transform>()} {
  OnEnter();
}

dae::EntityStateAttacking::~EntityStateAttacking() {}

std::unique_ptr<EntityState> dae::EntityStateAttacking::HandleInput() {
  auto input{m_Entity->GetInput()};
  auto level{m_Entity->GetLevel()};
  auto worldPos{m_Entity->GetEntity()->GetWorldPosition()};
  glm::vec2 charSize{32.f, 32.f};
  auto deltaTime{TimeManager::GetInstance().GetDeltaTime()};
  m_StopWatch += deltaTime;
  auto moveSpeed{m_Entity->GetMoveSpeed()};
  m_MovementVector = input->GetMovementDirection() * moveSpeed * deltaTime;
  // if we're not going to be on a ladder after moving we shouldnt move
  // vertically at all
  if (!level->IsOnLadder(worldPos + m_MovementVector, charSize) &&
      !level->IsOnLadder(worldPos + glm::vec2{0.f, 10.f}, charSize))
    m_MovementVector.y = 0.f;

  // if we're not going to be on a platform after moving we shouldnt move
  // horizontally at all
  if (!level->IsOnPlatform(worldPos + m_MovementVector, charSize))
    m_MovementVector.x = 0.f;

  // check if the direction changed
  auto newDir{VecToDirection(m_MovementVector)};
  // ignore neutral here
  if (m_CurrentFaceDir != newDir && newDir != Direction::Neutral) {
    m_CurrentFaceDir = newDir;
    SetAnimationState();
  }

  // set the direction to 1-axis (so you can't move diagonally)
  m_MovementVector = DirectionToVec(newDir) * moveSpeed;

  // if we've reached the attack time
  if (m_StopWatch > m_AttackTime) {
    auto currentScene{SceneManager::GetInstance().GetActiveScene()};
    currentScene->Remove(*m_Pepper);
    switch (newDir) {
    case Direction::Up:
    case Direction::Down:
      return std::make_unique<EntityStateClimbing>(*m_Entity, m_CurrentFaceDir);
    case Direction::Left:
    case Direction::Right:
      return std::make_unique<EntityStateWalking>(*m_Entity, m_CurrentFaceDir);
    case Direction::Neutral:
      return std::make_unique<EntityStateIdle>(*m_Entity, m_CurrentFaceDir);
    }
  }
  return nullptr;
}

void dae::EntityStateAttacking::Update() {
  m_EntityTransform->SetLocalPosition(
      m_EntityTransform->GetLocalPosition() +
      (m_MovementVector * TimeManager::GetInstance().GetDeltaTime()));
}

void dae::EntityStateAttacking::OnEnter() {
  SetAnimationState();
  CreatePepper();
}

void dae::EntityStateAttacking::OnExit() {}

Direction dae::EntityStateAttacking::VecToDirection(glm::vec2 const &vec) {
  if (vec.y > 0.f)
    return Direction::Down;
  if (vec.y < 0.f)
    return Direction::Up;
  if (vec.x > 0.f)
    return Direction::Right;
  if (vec.x < 0.f)
    return Direction::Left;
  return Direction::Neutral;
}

glm::vec2 dae::EntityStateAttacking::DirectionToVec(Direction dir) {
  if (dir == Direction::Up)
    return glm::vec2{0, -1.f};
  if (dir == Direction::Down)
    return glm::vec2{0, 1.f};
  if (dir == Direction::Left)
    return glm::vec2{-1.f, 0};
  if (dir == Direction::Right)
    return glm::vec2{1.f, 0};
  // neutral dir
  return glm::vec2{};
}

void dae::EntityStateAttacking::SetAnimationState() {
  switch (m_CurrentFaceDir) {
  case Direction::Up:
    m_Entity->GetEntityAnimation()->SetAnimationState("AttackUp");
    break;
  case Direction::Down:
    m_Entity->GetEntityAnimation()->SetAnimationState("AttackDown");
    break;
  case Direction::Left:
    m_Entity->GetEntityAnimation()->SetAnimationState("AttackLeft");
    break;
  case Direction::Right:
    m_Entity->GetEntityAnimation()->SetAnimationState("AttackRight");
    break;
  default:
    m_Entity->GetEntityAnimation()->SetAnimationState("AttackDown");
  }
}

void dae::EntityStateAttacking::CreatePepper() {
  glm::vec2 offset{};
  glm::vec2 charSize{32.f, 32.f};
  if (m_CurrentFaceDir == Direction::Up)
    offset = glm::vec2{0, -charSize.y * 0.75};
  if (m_CurrentFaceDir == Direction::Down)
    offset = glm::vec2{0, charSize.y * 0.75};
  if (m_CurrentFaceDir == Direction::Left)
    offset = glm::vec2{-charSize.x * 1.5, 0};
  if (m_CurrentFaceDir == Direction::Right)
    offset = glm::vec2{charSize.x * 1.5, 0};

  auto pepper = std::make_unique<GameObject>();
  pepper->AddComponent<ObjectRenderer>();
  pepper->AddComponent<Texture2DDisplay>("Data/Characters/Pepper.png");
  pepper->GetComponent<Transform>()->SetLocalPosition(
      m_Entity->GetEntity()->GetWorldPosition() + offset);
  pepper->AddComponent<RectCollider>(Rect{{}, {16, 32}}, LAYER_PEPPER,
                                     LAYER_NONE);
  m_Pepper = pepper.get();
  auto currentScene{SceneManager::GetInstance().GetActiveScene()};
  currentScene->Add(std::move(pepper));
}
