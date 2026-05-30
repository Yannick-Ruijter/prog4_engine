#include "GameObject.hpp"
#include "LayerPartComponent.hpp"
#include "TransformComponent.hpp"
#include "burgerLayerComponent.hpp"
using namespace dae;

int LayerPartComponent::CollisionDistanceSquared = 1000;

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
    if (m_IsSteppedOn)
        return;

    for (auto const &transform : m_PlayerTransforms) {
        auto playerWorldPos = transform->GetWorldPosition();
        auto pos = m_Transform->GetWorldPosition();
        float distSqr = glm::dot(pos - playerWorldPos, pos - playerWorldPos);
        if (distSqr <= CollisionDistanceSquared) {
            GetOwner()->GetParent()->GetComponent<BurgerLayerComponent>();
            m_IsSteppedOn = true;
            auto localPos = m_Transform->GetLocalPosition();
            float steppedOnOffset{5.f};
            localPos.y += steppedOnOffset;
            m_Transform->SetLocalPosition(localPos);
        }
    }
}

bool LayerPartComponent::IsSteppedOn() const {
    return m_IsSteppedOn;
}

void dae::LayerPartComponent::Startfalling() const {
}
