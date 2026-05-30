#include "GameObject.hpp"
#include "LayerPartComponent.hpp"
#include "TransformComponent.hpp"
#include "burgerLayerComponent.hpp"
using namespace dae;

int LayerPartComponent::CollisionDistanceSquared = 500;
float LayerPartComponent::SteppedOnOffset = 5.f;
void LayerPartComponent::SetCollisionDistance(int dist) {
    CollisionDistanceSquared = dist * dist;
}

LayerPartComponent::LayerPartComponent(GameObject &owner, std::vector<GameObject *> const &players)
    : Component{owner}, m_Transform{owner.GetComponent<TransformComponent>()} {
    for (auto const &player : players) {
        m_PlayerTransforms.emplace_back(player->GetComponent<TransformComponent>());
    }
}

void LayerPartComponent::Update() {
    if (m_IsSteppedOn || m_IsFalling)
        return;

    for (auto const &transform : m_PlayerTransforms) {
        auto playerWorldPos = transform->GetWorldPosition();
        auto pos = m_Transform->GetWorldPosition();
        float distSqr = glm::dot(pos - playerWorldPos, pos - playerWorldPos);
        if (distSqr <= CollisionDistanceSquared) {
            m_IsSteppedOn = true;
            auto localPos = m_Transform->GetLocalPosition();
            localPos.y += SteppedOnOffset;
            m_Transform->SetLocalPosition(localPos);
            GetOwner()->GetParent()->GetComponent<BurgerLayerComponent>()->OnLayerPartCollided();
        }
    }
}

bool LayerPartComponent::IsSteppedOn() const {
    return m_IsSteppedOn;
}

void dae::LayerPartComponent::SetFallingState(bool state) {
    m_IsFalling = state;
    if (!state)
        m_IsSteppedOn = false;
}
