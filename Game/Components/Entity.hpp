#pragma once

#include "Component.hpp"
#include "GameStructs.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
#include <memory>
namespace dae {
class SpriteAnimation;
class EntityState;
class InputProvider;
class LevelGrid;
class RectCollider;
class Entity final : public Component {
public:
  Entity(GameObject &owner, std::unique_ptr<InputProvider> input,
         LevelGrid *level, Character character, float speed = 100.f);
  ~Entity();
  void Update();
  GameObject *GetEntity();
  SpriteAnimation *GetEntityAnimation() const;
  InputProvider *GetInput() const;
  LevelGrid *GetLevel() const;
  Character GetCharacterType() const;
  float GetMoveSpeed() const;
  void NotifyFromState(EventId event);
  Subject *GetDeathEvent() const;
  Subject *GetAttackEvent() const;

private:
  Character m_Character;
  SpriteAnimation *m_SpriteAnimation;
  std::unique_ptr<EntityState> m_CurrentState;
  std::unique_ptr<InputProvider> m_Input;
  LevelGrid *m_Level;
  float m_MoveSpeed{100.f};
  std::unique_ptr<Subject> m_DeathEvent{std::make_unique<Subject>()};
  std::unique_ptr<Subject> m_AttackEvent{std::make_unique<Subject>()};
};
} // namespace dae
