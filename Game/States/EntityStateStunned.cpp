#include "CustomCommands.hpp"
#include "Entity.hpp"
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

std::unique_ptr<EntityState> dae::EntityStateStunned::HandleInput() {
  m_StopWatch += TimeManager::GetInstance().GetDeltaTime();
  // if we've been stunned for long enough
  if (m_StopWatch > m_StunnedTime) {
    return std::make_unique<EntityStateIdle>(*m_Entity, Direction::Down);
  }
  return nullptr;
}

void dae::EntityStateStunned::OnEnter() {
  m_Entity->GetEntityAnimation()->SetAnimationState("Stunned");
}
