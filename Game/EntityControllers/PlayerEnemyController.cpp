#include "GameObject.hpp"
#include "PlayerEnemyController.hpp"
#include "PlayerInput.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"

dae::PlayerEnemyController::PlayerEnemyController(
    std::vector<PlayerInput *> inputs, GameObject *controlledEnemy)
    : m_Inputs{inputs}, m_ControlledEnemy{controlledEnemy} {}

dae::PlayerEnemyController::~PlayerEnemyController() {}

glm::vec2 dae::PlayerEnemyController::GetMovementDirection() const {
  glm::vec2 dir{};
  for (auto const &input : m_Inputs) {
    if (input->IsButtonPressed(InputAction::MoveUp))
      dir.y = 1.f;
    if (input->IsButtonPressed(InputAction::MoveDown))
      dir.y = 1.f;
    if (input->IsButtonPressed(InputAction::MoveLeft))
      dir.x = 1.f;
    if (input->IsButtonPressed(InputAction::MoveRight))
      dir.x = 1.f;
  }
  return dir;
}

bool dae::PlayerEnemyController::AttackButtonPressed() const { return false; }

bool dae::PlayerEnemyController::IsStunned() {
  bool stunned = m_HasBeenStunned;
  m_HasBeenStunned = false; // reset after being polled
  return stunned;
}

bool dae::PlayerEnemyController::ShouldDie() const { return m_ShouldDie; }

void dae::PlayerEnemyController::Notify(EventId eventId, GameObject *) {
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
}
