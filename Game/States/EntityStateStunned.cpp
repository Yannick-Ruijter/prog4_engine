#include "CustomCommands.hpp"
#include "Entity.hpp"
#include "EntityStateDeath.hpp"
#include "EntityStateIdle.hpp"
#include "EntityStateStunned.hpp"
#include "GameObject.hpp"
#include "InputProvider.hpp"
#include "LevelGrid.hpp"
#include "ObjectRenderer.hpp"
#include "RectCollider.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "SpriteAnimation.hpp"
#include "TimeManager.hpp"

using namespace dae;

dae::EntityStateStunned::EntityStateStunned(Entity &entity)
    : EntityState(entity),
      m_EntityTransform{entity.GetEntity()->GetComponent<dae::Transform>()} {
  OnEnter();
}

dae::EntityStateStunned::~EntityStateStunned() { OnExit(); }

std::unique_ptr<EntityState> dae::EntityStateStunned::HandleInput() {
  m_StopWatch += TimeManager::GetInstance().GetDeltaTime();
  if (m_Entity->GetInput()->ShouldDie())
    return std::make_unique<EntityStateDying>(*m_Entity);
  // if we've been stunned for long enough
  if (m_StopWatch > m_StunnedTime) {
    return std::make_unique<EntityStateIdle>(*m_Entity, Direction::Down);
  }
  return nullptr;
}

void dae::EntityStateStunned::OnEnter() {
  m_Entity->GetEntityAnimation()->SetAnimationState("Stunned");
  m_Collider = m_Entity->GetEntity()->GetComponent<RectCollider>();
  m_Collider->SetActiveState(false);
}

void dae::EntityStateStunned::OnExit() { m_Collider->SetActiveState(true); }
