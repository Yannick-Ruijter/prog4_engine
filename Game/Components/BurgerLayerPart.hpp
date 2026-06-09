#pragma once

#include "Component.hpp"
#include "Observer.hpp"
#include <vector>

namespace dae {
class GameObject;
class Transform;
class BurgerLayer;
class BurgerLayerPart : public Component, public Observer {
public:
  static void SetCollisionDistance(int dist);
  static float SteppedOnOffset;
  BurgerLayerPart(GameObject &owner);
  bool IsSteppedOn() const;
  void SetFallingState(bool state);

  virtual void Notify(EventId eventId, GameObject *source) override;

private:
  static int CollisionDistanceSquared;
  bool m_IsSteppedOn{false};
  bool m_IsFalling{false};
  Transform *m_Transform{};

  // Inherited via Observer
};
} // namespace dae
