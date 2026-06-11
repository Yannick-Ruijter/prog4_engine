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
#include <Subject.hpp>
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
  m_MovementVec = glm::vec2{1.f, 0.f};
}

dae::AIEnemyController::~AIEnemyController() {}
glm::vec2 dae::AIEnemyController::GetMovementDirection() {
  if (m_TargetPlayer == nullptr)
    return {};

  constexpr float displacement = 4.f;
  constexpr float ladderOffset = 10.f;
  const glm::vec2 charSize{32.f, 32.f};

  auto *level = m_Manager->GetLevel();
  const int tileSize = level->GetGridSize();
  const glm::vec2 worldPos = m_Transform->GetWorldPosition();

  // check what directions we can go to
  const bool canGoUp =
      level->IsOnLadder(worldPos + glm::vec2{0, -displacement}, charSize) ||
      level->IsOnLadder(worldPos + glm::vec2{0, -displacement + ladderOffset},
                        charSize);
  const bool canGoDown = level->IsOnLadder(
      worldPos + glm::vec2{0, displacement + ladderOffset}, charSize);
  const bool canGoLeft =
      level->IsOnPlatform(worldPos + glm::vec2{-displacement, 0}, charSize);
  const bool canGoRight =
      level->IsOnPlatform(worldPos + glm::vec2{displacement, 0}, charSize);

  // compensating for my bad programming (double checking if we can move in a
  // direction
  const bool tileLeft =
      level->IsPlatform(level->GetTile(worldPos + glm::vec2{-tileSize, 0}));
  const bool tileRight =
      level->IsPlatform(level->GetTile(worldPos + glm::vec2{tileSize, 0}));
  const bool tileBelow =
      level->IsLadder(level->GetTile(worldPos + glm::vec2{0, tileSize}));

  const bool allowUp = canGoUp;
  const bool allowDown = canGoDown && tileBelow;
  const bool allowLeft = canGoLeft && tileLeft;
  const bool allowRight = canGoRight && tileRight;

  const bool atCrossroads = (allowUp || allowDown) && (allowLeft || allowRight);

  if (!atCrossroads && !m_FirstLoop) {
    m_WasOnCrossRoads = false;

    // Flip if we've hit a dead end in the current direction
    if ((!canGoUp && m_MovementVec.y < 0.f) ||
        (!canGoDown && m_MovementVec.y > 0.f))
      m_MovementVec.y = -m_MovementVec.y;

    if ((!canGoLeft && m_MovementVec.x < 0.f) ||
        (!canGoRight && m_MovementVec.x > 0.f))
      m_MovementVec.x = -m_MovementVec.x;

    return m_MovementVec;
  }

  // we don't want to evaluate the same crossroads multiple times in a row
  if (m_WasOnCrossRoads && !m_FirstLoop)
    return m_MovementVec;

  m_FirstLoop = false;
  m_WasOnCrossRoads = true;

  const glm::vec2 diff = m_TargetTransform->GetWorldPosition() - worldPos;

  // give it a chance to change direction randomly
  std::uniform_int_distribution<int> dist(0, 3);
  const bool doRandom =
      dist(ServiceProvider::GetRandomProvider().GetRng()) == 1;

  const bool movingH = m_MovementVec.x != 0.f;

  // creating the lambdas to set direction
  auto tryUp = [&] {
    if (allowUp && m_MovementVec.y <= 0.f) {
      m_MovementVec = {0, -1};
      return true;
    }
    return false;
  };
  auto tryDown = [&] {
    if (allowDown && m_MovementVec.y >= 0.f) {
      m_MovementVec = {0, 1};
      return true;
    }
    return false;
  };
  auto tryLeft = [&] {
    if (allowLeft && m_MovementVec.x <= 0.f) {
      m_MovementVec = {-1, 0};
      return true;
    }
    return false;
  };
  auto tryRight = [&] {
    if (allowRight && m_MovementVec.x >= 0.f) {
      m_MovementVec = {1, 0};
      return true;
    }
    return false;
  };

  if (doRandom) {
    // Pick a random valid perpendicular direction
    if (movingH) {
      if (rand() % 2)
        tryDown() || tryUp();
      else
        tryUp() || tryDown();
    } else {
      if (rand() % 2)
        tryLeft() || tryRight();
      else
        tryRight() || tryLeft();
    }
  } else {
    // try and change direction that brings closer to player
    bool moved = false;
    if (movingH) {
      // if we're moving horizontally, we try vertical moving first
      moved = (diff.y < 0 && tryUp()) || (diff.y > 0 && tryDown()) ||
              (diff.x < 0 && tryLeft()) || (diff.x > 0 && tryRight());
    } else {
      // if we're moving vertically, we try horizontal first
      moved = (diff.x < 0 && tryLeft()) || (diff.x > 0 && tryRight()) ||
              (diff.y < 0 && tryUp()) || (diff.y > 0 && tryDown());
    }

    // if we have not found a path in the direction of the player
    if (!moved) {
      // Prefer continuing in the same direction if we can't go to a direction
      // that brings us closer
      bool keptDir = false;
      if (m_MovementVec.y < 0.f)
        keptDir = tryUp();
      else if (m_MovementVec.y > 0.f)
        keptDir = tryDown();
      else if (m_MovementVec.x < 0.f)
        keptDir = tryLeft();
      else if (m_MovementVec.x > 0.f)
        keptDir = tryRight();

      // if we can't continue moving, just take the first fitting one
      if (!keptDir)
        tryUp() || tryDown() || tryLeft() || tryRight();
    }
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

  if (eventId == "OnEntityDeath"_h && source == m_ControlledEnemy) {
    if (m_TargetPlayer) {
      auto entity{m_TargetPlayer->GetComponent<Entity>()};
      if (entity)
        entity->GetDeathEvent()->RemoveObserver(this);
    }
  }

  if (eventId == "OnEntityDeath"_h && source == m_TargetPlayer) {
    m_TargetPlayer = m_Manager->GetRandomPlayer();
  }
}
