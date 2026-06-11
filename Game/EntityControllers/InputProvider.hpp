#pragma once
#include "Observer.hpp"
#include <glm/glm.hpp>
namespace dae {
class PlayerInput;
class InputProvider : public Observer {
public:
  virtual ~InputProvider() {};
  virtual glm::vec2 GetMovementDirection() { return glm::vec2{}; }
  virtual bool AttackButtonPressed() const { return false; }
  virtual bool IsStunned() { return false; }
  virtual void Notify(EventId, GameObject *) override {}
  virtual bool ShouldDie() const { return false; }
};
} // namespace dae
