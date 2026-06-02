#include "PlayerInput.hpp"
#include "PlayerInputProvider.hpp"

dae::PlayerInputProvider::PlayerInputProvider(PlayerInput *input) : m_Input{input} {
}

dae::PlayerInputProvider::~PlayerInputProvider() {
}
glm::vec2 dae::PlayerInputProvider::GetMovementDirection() const {
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

bool dae::PlayerInputProvider::AttackButtonPressed() const {
    return m_Input->WasPressedThisFrame(InputAction::Attack);
}
