#include "GameObject.hpp"
#include "PlayerEnemyController.hpp"
#include "PlayerInput.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"

dae::PlayerEnemyController::PlayerEnemyController(PlayerInput *input,
                                                  GameObject *controlledEnemy)
    : m_Input{input}, m_ControlledEnemy{controlledEnemy} {}

dae::PlayerEnemyController::~PlayerEnemyController() {}
glm::vec2 dae::PlayerEnemyController::GetMovementDirection() const {
  glm::vec2 dir{};
  if (m_Input->IsButtonPressed(InputAction::MoveUp))
    dir.y -= 1.f;
  if (m_Input->IsButtonPressed(InputAction::MoveDown))
    dir.y += 1.f;
  if (m_Input->IsButtonPressed(InputAction::MoveLeft))
    dir.x -= 1.f;
  if (m_Input->IsButtonPressed(InputAction::MoveRight))
    dir.x += 1.f;
  return dir;
}

bool dae::PlayerEnemyController::AttackButtonPressed() const { return false; }

void dae::PlayerEnemyController::Notify(EventId eventId, GameObject *) {
  if (!m_Collider)
    m_Collider = m_ControlledEnemy->GetComponent<RectCollider>();
  if (eventId == "OnCollision"_h) {
    auto other = m_Collider->GetLastCollision();
    if (!other)
      return;
    if (auto layer = other->GetOwner()->GetComponent<BurgerLayer>();
        layer && layer->IsFalling()) {
      SceneManager::GetInstance().GetActiveScene()->Remove(*m_ControlledEnemy);
    }
    if (other->GetLayer() == LAYER_PEPPER) {
      SceneManager::GetInstance().GetActiveScene()->Remove(*m_ControlledEnemy);
    }
  }
}
