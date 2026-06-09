#include "PlayerEnemyController.hpp"
#include "PlayerInput.hpp"

dae::PlayerEnemyController::PlayerEnemyController(PlayerInput *input)
    : m_Input{input} {}

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
