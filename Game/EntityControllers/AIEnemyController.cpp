#include "AIEnemyController.hpp"
#include "PlayerInput.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"
#include <Entity.hpp>
#include <GameManager.hpp>
#include <GameObject.hpp>
#include <LevelGrid.hpp>
#include <ServiceProvider.hpp>
#include <TimeManager.hpp>
#include <Transform.hpp>
#include <random>

dae::AIEnemyController::AIEnemyController(GameObject *controlledEnemy,
                                          GameManager *manager)
    : m_ControlledEnemy{controlledEnemy}, m_Manager{manager} {
  m_Transform = controlledEnemy->GetComponent<Transform>();
  m_TargetPlayer = m_Manager->GetRandomPlayer();
  auto entity{m_TargetPlayer->GetComponent<Entity>()};
  entity->GetDeathEvent()->AddObserver(this);
  m_Speed = entity->GetMoveSpeed();
  m_TargetTransform = m_TargetPlayer->GetComponent<Transform>();
  // force 2 directions so the first time something gets the input, it checks
  // for both axis
  m_MovementVec = glm::vec2{1.f, 1.f};
}

dae::AIEnemyController::~AIEnemyController() {}

glm::vec2 dae::AIEnemyController::GetMovementDirection() {
  // if we have no target, we can just stay still
  if (m_TargetPlayer == nullptr)
    return glm::vec2{};

  auto deltaTime{TimeManager::GetInstance().GetDeltaTime()};
  float displacement{deltaTime * m_Speed};
  glm::vec2 charSize{32.f, 32.f};
  auto worldPos{m_Transform->GetWorldPosition()};

  // check possible direction to go to
  // clang-format off
  //we can go up when we're either on a ladder or slightly above one (to reach the platform since it's not perfectly aligned with each cell)
  auto level = m_Manager->GetLevel();
  bool canGoUp =
      level->IsOnLadder(worldPos + glm::vec2{0, -displacement}, charSize) || level->IsOnLadder(worldPos + glm::vec2{ 0, -displacement + 10.f }, charSize);
  bool canGoDown =
      level->IsOnLadder(worldPos + glm::vec2{0, displacement + 10.f}, charSize);
  bool canGoLeft =
      level->IsOnPlatform(worldPos + glm::vec2{-displacement, 0}, charSize);
  bool canGoRight =
      level->IsOnPlatform(worldPos + glm::vec2{displacement, 0}, charSize);
  // clang-format on
  bool atCrossRoads{(canGoUp || canGoDown) && (canGoLeft || canGoRight)};
  if (atCrossRoads || m_FirstLoop) {
    m_FirstLoop = false;
    if (m_WasOnCrossRoads)
      return m_MovementVec;
    m_WasOnCrossRoads = true;
    auto targetPos{m_TargetTransform->GetWorldPosition()};
    auto diff{targetPos - worldPos};
    // switch axis so i give priority to changing direction
    glm::vec2 temp{m_MovementVec.y * m_MovementVec.y,
                   m_MovementVec.x * m_MovementVec.x};
    temp = temp * diff;
    // check for each direction if we can walk/climb  there and if we're not
    // going the opposite direction

    if (temp.y < 0 && canGoUp && m_MovementVec.y <= 0.f)
      m_MovementVec = glm::vec2{0, -1.f};
    else if (temp.y > 0 && canGoDown && m_MovementVec.y >= 0.f)
      m_MovementVec = glm::vec2{0, 1.f};
    else if (temp.x < 0 && canGoLeft && m_MovementVec.x <= 0.f)
      m_MovementVec = glm::vec2{-1.f, 0};
    else if (temp.x > 0 && canGoRight && m_MovementVec.x >= 0.f)
      m_MovementVec = glm::vec2{1.f, 0};
    else {
      std::uniform_int_distribution<int> dist(0, 3);
      // 25% chance to change direction randomly
      auto result = dist(ServiceProvider::GetRandomProvider().GetRng());
      if (result == 0) {
        // is moving vertically
        bool isMovingVertically{m_MovementVec.y != 0.f};
        bool isMovingHorizontally{m_MovementVec.x != 0.f};
        if (canGoUp && !isMovingVertically)
          m_MovementVec = glm::vec2{0, -1.f};
        else if (canGoDown && !isMovingVertically)
          m_MovementVec = glm::vec2{0, 1.f};
        else if (canGoLeft && !isMovingHorizontally)
          m_MovementVec = glm::vec2{-1.f, 0.f};
        else if (canGoRight && !isMovingHorizontally)
          m_MovementVec = glm::vec2{1.f, 0.f};
      }
      return m_MovementVec;
    }
  } else {
    m_WasOnCrossRoads = false;
    // if we have reached a vertical dead end, we turn around (because we're not
    // at a crossroads
    if ((!(canGoUp) && m_MovementVec.y < 0.f) ||
        (!canGoDown && m_MovementVec.y > 0.f))
      m_MovementVec.y = -m_MovementVec.y;
    // if we have reached a horizontal dead end, we turn around (because we're
    // not at a crossroads
    if ((!canGoLeft && m_MovementVec.x < 0.f) ||
        (!canGoRight && m_MovementVec.x > 0.f))
      m_MovementVec.x = -m_MovementVec.x;
  }
  return m_MovementVec;
}

bool dae::AIEnemyController::AttackButtonPressed() const { return false; }

bool dae::AIEnemyController::IsStunned() {
  bool stunned = m_HasBeenStunned;
  m_HasBeenStunned = false; // reset after being polled
  return stunned;
}

bool dae::AIEnemyController::ShouldDie() const { return m_ShouldDie; }

void dae::AIEnemyController::Notify(EventId eventId, GameObject *source) {
  if (!m_Collider)
    m_Collider = m_ControlledEnemy->GetComponent<RectCollider>();
  if (eventId == "OnCollision"_h) {
    auto other = m_Collider->GetLastCollision();
    if (!other)
      return;
    if (auto layer = other->GetOwner()->GetComponent<BurgerLayer>();
        layer && layer->IsFalling()) {
      m_ShouldDie = true;
    }
    if (other->GetLayer() == LAYER_PEPPER) {
      m_HasBeenStunned = true;
    }
  }

  if (eventId == "OnEntityDeath"_h && source == m_TargetPlayer) {
    m_TargetPlayer = m_Manager->GetRandomPlayer();
  }
}
