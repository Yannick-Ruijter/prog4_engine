#include "PlayerInput.hpp"
#include "PlayerPepperController.hpp"

dae::PlayerPepperController::PlayerPepperController(PlayerInput *input)
    : m_Input{input} {}

dae::PlayerPepperController::~PlayerPepperController() {}
glm::vec2 dae::PlayerPepperController::GetMovementDirection() const {
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

bool dae::PlayerPepperController::AttackButtonPressed() const {
  return m_Input->WasPressedThisFrame(InputAction::Attack);
}
