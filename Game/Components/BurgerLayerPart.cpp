#include "BurgerLayerPart.hpp"
#include "GameObject.hpp"
#include "ServiceProvider.hpp"
#include "SoundSystem.hpp"
#include "Transform.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

int BurgerLayerPart::CollisionDistanceSquared = 500;
float BurgerLayerPart::SteppedOnOffset = 5.f;
void BurgerLayerPart::SetCollisionDistance(int dist) {
  CollisionDistanceSquared = dist * dist;
}

BurgerLayerPart::BurgerLayerPart(GameObject &owner)
    : Component{owner}, m_Transform{owner.GetComponent<Transform>()} {}

bool BurgerLayerPart::IsSteppedOn() const { return m_IsSteppedOn; }

void dae::BurgerLayerPart::SetFallingState(bool state) {
  m_IsFalling = state;
  if (!state)
    m_IsSteppedOn = false;
}

void dae::BurgerLayerPart::Notify(EventId eventId, GameObject *) {
  if (eventId == "OnCollision"_h && !m_IsSteppedOn && !m_IsFalling) {
    sound_id burgerStepId{3};
    ServiceProvider::GetSoundSystem().Play(burgerStepId, 0.2f);
    m_IsSteppedOn = true;
    auto localPos = m_Transform->GetLocalPosition();
    localPos.y += SteppedOnOffset;
    m_Transform->SetLocalPosition(localPos);
    GetOwner()->GetParent()->GetComponent<BurgerLayer>()->OnLayerPartCollided();
  }
}
