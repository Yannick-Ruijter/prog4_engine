#include "BurgerLayerPart.hpp"
#include "GameObject.hpp"
#include "ServiceProvider.hpp"
#include "SoundSystem.hpp"
#include "Transform.hpp"
#include "burgerLayer.hpp"
using namespace dae;

int BurgerLayerPart::CollisionDistanceSquared = 500;
float BurgerLayerPart::SteppedOnOffset = 5.f;
void BurgerLayerPart::SetCollisionDistance(int dist) {
    CollisionDistanceSquared = dist * dist;
}

BurgerLayerPart::BurgerLayerPart(GameObject &owner, std::vector<GameObject *> const &players)
    : Component{owner}, m_Transform{owner.GetComponent<Transform>()} {
    for (auto const &player : players) {
        m_PlayerTransforms.emplace_back(player->GetComponent<Transform>());
    }
}

void BurgerLayerPart::Update() {
    if (m_IsSteppedOn || m_IsFalling)
        return;

    // compare the distance to all players
    for (auto const &transform : m_PlayerTransforms) {
        auto playerWorldPos = transform->GetWorldPosition();
        auto pos = m_Transform->GetWorldPosition();
        float distSqr = glm::dot(pos - playerWorldPos, pos - playerWorldPos);
        if (distSqr <= CollisionDistanceSquared) {
            sound_id burgerStepId{3};
            ServiceProvider::GetSoundSystem().Play(burgerStepId, 0.2f);
            m_IsSteppedOn = true;
            auto localPos = m_Transform->GetLocalPosition();
            localPos.y += SteppedOnOffset;
            m_Transform->SetLocalPosition(localPos);
            GetOwner()->GetParent()->GetComponent<BurgerLayer>()->OnLayerPartCollided();
        }
    }
}

bool BurgerLayerPart::IsSteppedOn() const {
    return m_IsSteppedOn;
}

void dae::BurgerLayerPart::SetFallingState(bool state) {
    m_IsFalling = state;
    if (!state)
        m_IsSteppedOn = false;
}
