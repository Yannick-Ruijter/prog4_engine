#include "GameObject.hpp"
#include "PlayerInput.hpp"
#include "PlayerPepperController.hpp"
#include "RectCollider.hpp"
#include "sdbm_hash.hpp"

dae::PlayerPepperController::PlayerPepperController(
    std::vector<PlayerInput *> inputs, GameObject *controlledCharacter,
    int pepperCount)
    : m_Inputs{inputs}, m_ControlledPlayer{controlledCharacter},
      m_PepperCount{pepperCount} {}

dae::PlayerPepperController::~PlayerPepperController() {}
glm::vec2 dae::PlayerPepperController::GetMovementDirection() const {
  glm::vec2 dir{};
  for (auto const &input : m_Inputs) {
    if (input->IsButtonPressed(InputAction::MoveUp) && dir.y != -1.f)
      dir.y -= 1.f;
    if (input->IsButtonPressed(InputAction::MoveDown) && dir.y != 1.f)
      dir.y += 1.f;
    if (input->IsButtonPressed(InputAction::MoveLeft) && dir.x != -1.f)
      dir.x -= 1.f;
    if (input->IsButtonPressed(InputAction::MoveRight) && dir.x != 1.f)
      dir.x += 1.f;
  }
  return dir;
}

bool dae::PlayerPepperController::AttackButtonPressed() const {
  if (m_PepperCount == 0)
    return false;
  for (auto const &input : m_Inputs) {
    if (input->WasPressedThisFrame(InputAction::Attack))
      return true;
  }
  return false;
}

bool dae::PlayerPepperController::IsStunned() { return false; }

bool dae::PlayerPepperController::ShouldDie() const { return m_ShouldDie; }

void dae::PlayerPepperController::Notify(EventId event, GameObject *) {
  if (!m_Collider)
    m_Collider = m_ControlledPlayer->GetComponent<RectCollider>();
  if (event == "Attacked"_h) {
    m_PepperCount--;
  }
  if (event == "OnCollision"_h) {
    auto other = m_Collider->GetLastCollision();
    if (!other)
      return;
    if (other->GetLayer() == LAYER_ENEMY) {
      m_ShouldDie = true;
    }
  }
}

int dae::PlayerPepperController::GetPepperCount() const {
  return m_PepperCount;
}
