#include "Entity.hpp"
#include "EntityState.hpp"
#include "EntityStateIdle.hpp"
#include "GameObject.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "RectCollider.hpp"
#include "SpriteAnimation.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

Entity::Entity(GameObject &owner, std::unique_ptr<InputProvider> input,
               LevelGrid *level)
    : Component(owner),
      m_SpriteAnimation{owner.GetComponent<SpriteAnimation>()},
      m_CurrentState{std::make_unique<EntityStateIdle>(*this)},
      m_Input{std::move(input)}, m_Level{level} {
  assert(m_SpriteAnimation != nullptr &&
         "Entity needs a SpriteAnimation (added before this component)");
}

dae::Entity::~Entity() {}

void dae::Entity::Update() {
  auto tempState = m_CurrentState->HandleInput();
  if (tempState != nullptr)
    m_CurrentState = std::move(tempState);
  m_CurrentState->Update();
}

GameObject *dae::Entity::GetEntity() { return GetOwner(); }

SpriteAnimation *dae::Entity::GetPlayerAnimation() const {
  return m_SpriteAnimation;
}

InputProvider *dae::Entity::GetInput() const { return m_Input.get(); }

LevelGrid *dae::Entity::GetLevel() const { return m_Level; }

float dae::Entity::GetMoveSpeed() const { return m_MoveSpeed; }
