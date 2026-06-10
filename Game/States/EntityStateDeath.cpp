#include "CustomCommands.hpp"
#include "Entity.hpp"
#include "EntityStateDeath.hpp"
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

dae::EntityStateDying::EntityStateDying(Entity &entity)
    : EntityState(entity),
      m_EntityTransform{entity.GetEntity()->GetComponent<dae::Transform>()} {
  OnEnter();
}

std::unique_ptr<EntityState> dae::EntityStateDying::HandleInput() {
  m_StopWatch += TimeManager::GetInstance().GetDeltaTime();
  // if we've been dying for long enough
  if (m_StopWatch > m_StunnedTime) {
    OnExit();
  }
  return nullptr;
}

void dae::EntityStateDying::OnEnter() {
  m_Entity->GetEntityAnimation()->SetAnimationState("Dying");
  m_Collider = m_Entity->GetEntity()->GetComponent<RectCollider>();
  m_Collider->SetActiveState(false);
}

void dae::EntityStateDying::OnExit() {
  auto scene = SceneManager::GetInstance().GetActiveScene();
  m_Entity->NotifyFromState("OnEntityDeath"_h);
  scene->Remove(*m_Entity->GetEntity());
}
