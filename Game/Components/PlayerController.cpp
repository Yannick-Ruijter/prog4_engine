#include "GameObject.hpp"
#include "LevelGrid.hpp"
#include "PlayerAnimation.hpp"
#include "PlayerController.hpp"
#include "PlayerInput.hpp"
#include "PlayerState.hpp"
#include "PlayerStateIdle.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

PlayerController::PlayerController(GameObject &owner, PlayerInput *input, LevelGrid *level)
    : Component(owner),
      m_PlayerAnimation{owner.GetComponent<PlayerAnimation>()},
      m_CurrentState{std::make_unique<PlayerStateIdle>(*this)},
      m_PlayerInput{input},
      m_Level{level} {
    assert(
        m_PlayerAnimation != nullptr &&
        "PlayerController needs a PlayerAnimation (added before this component)");
}

dae::PlayerController::~PlayerController() {
}

void dae::PlayerController::Update() {
    auto tempState = m_CurrentState->HandleInput();
    if (tempState != nullptr)
        m_CurrentState = std::move(tempState);
    m_CurrentState->Update();
}

GameObject *dae::PlayerController::GetPlayer() {
    return GetOwner();
}

PlayerAnimation *dae::PlayerController::GetPlayerAnimation() const {
    return m_PlayerAnimation;
}

PlayerInput *dae::PlayerController::GetInput() const {
    return m_PlayerInput;
}

LevelGrid *dae::PlayerController::GetLevel() const {
    return m_Level;
}

float dae::PlayerController::GetMoveSpeed() const {
    return m_MoveSpeed;
}
