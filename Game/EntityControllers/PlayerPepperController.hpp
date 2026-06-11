#pragma once

#include "InputProvider.hpp"
#include <vector>
namespace dae {
class PlayerInput;
class GameObject;
class RectCollider;
class PlayerPepperController : public InputProvider {
public:
  PlayerPepperController(std::vector<PlayerInput *> inputs,
                         GameObject *controlledCharacter, int pepperCount);
  virtual ~PlayerPepperController() override;
  virtual glm::vec2 GetMovementDirection() override;
  virtual bool AttackButtonPressed() const override;
  virtual bool IsStunned() override;
  virtual bool ShouldDie() const override;
  virtual void Notify(EventId, GameObject *) override;
  int GetPepperCount() const;

private:
  std::vector<PlayerInput *> m_Inputs{nullptr};
  GameObject *m_ControlledPlayer{nullptr};
  RectCollider *m_Collider{nullptr};
  int m_PepperCount{};
  bool m_ShouldDie{false};
};
} // namespace dae
