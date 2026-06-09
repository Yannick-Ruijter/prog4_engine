#pragma once
#include "Observer.hpp"
#include <glm/glm.hpp>
namespace dae {
class PlayerInput;
class InputProvider : public Observer {
public:
  virtual ~InputProvider() {};
  virtual glm::vec2 GetMovementDirection() const = 0;
  virtual bool AttackButtonPressed() const = 0;
  virtual bool IsStunned() = 0;
  virtual void Notify(EventId, GameObject *) override {}
};
} // namespace dae
