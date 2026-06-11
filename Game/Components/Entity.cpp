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
               LevelGrid *level, Character character, float speed)
    : Component(owner), m_Character{character},
      m_SpriteAnimation{owner.GetComponent<SpriteAnimation>()},
      m_CurrentState{std::make_unique<EntityStateIdle>(*this)},
      m_Input{std::move(input)}, m_Level{level}, m_MoveSpeed{speed} {
  assert(m_SpriteAnimation != nullptr &&
         "Entity needs a SpriteAnimation (added before this component)");
  m_AttackEvent->AddObserver(m_Input.get());
  m_DeathEvent->AddObserver(m_Input.get());
}

dae::Entity::~Entity() {}

void dae::Entity::Update() {
  auto tempState = m_CurrentState->HandleInput();
  if (tempState != nullptr)
    m_CurrentState = std::move(tempState);
  m_CurrentState->Update();
}

GameObject *dae::Entity::GetEntity() { return GetOwner(); }

SpriteAnimation *dae::Entity::GetEntityAnimation() const {
  return m_SpriteAnimation;
}

InputProvider *dae::Entity::GetInput() const { return m_Input.get(); }

LevelGrid *dae::Entity::GetLevel() const { return m_Level; }

Character dae::Entity::GetCharacterType() const { return m_Character; }
float dae::Entity::GetMoveSpeed() const { return m_MoveSpeed; }

void dae::Entity::NotifyFromState(EventId event) {
  if (event == "Attacked"_h)
    m_AttackEvent->NotifyObservers(event, GetOwner());
  if (event == "OnEntityDeath"_h)
    m_DeathEvent->NotifyObservers(event, GetOwner());
}

Subject *dae::Entity::GetDeathEvent() const { return m_DeathEvent.get(); }
Subject *dae::Entity::GetAttackEvent() const { return m_AttackEvent.get(); }
