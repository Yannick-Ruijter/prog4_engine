#pragma once

#include "Component.hpp"
#include "Observer.hpp"
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
         LevelGrid *level);
  ~Entity();
  void Update();
  GameObject *GetEntity();
  SpriteAnimation *GetPlayerAnimation() const;
  InputProvider *GetInput() const;
  LevelGrid *GetLevel() const;
  float GetMoveSpeed() const;

private:
  SpriteAnimation *m_SpriteAnimation;
  std::unique_ptr<EntityState> m_CurrentState;
  std::unique_ptr<InputProvider> m_Input;
  LevelGrid *m_Level;
  float m_MoveSpeed{100.f};
};
} // namespace dae
