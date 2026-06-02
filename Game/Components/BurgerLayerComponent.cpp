#include "GameObject.hpp"
#include "LayerPartComponent.hpp"
#include "LevelGridComponent.hpp"
#include "RenderComponent.hpp"
#include "Texture2DComponent.hpp"
#include "TimeManager.hpp"
#include "burgerLayerComponent.hpp"
using namespace dae;

std::vector<GameObject *> BurgerLayerComponent::AllBurgerLayers = {};
BurgerLayerComponent::BurgerLayerComponent(
    GameObject &owner, BurgerLayer layer, std::vector<GameObject *> const &players, LevelGridComponent *levelGrid)
    : Component(owner), m_LevelGrid{levelGrid}, m_Transform{owner.GetComponent<TransformComponent>()} {
    AllBurgerLayers.emplace_back(GetOwner());
    CreateChildrenParts(players, layer);
}

void BurgerLayerComponent::OnLayerPartCollided() {
    ++m_NumberOfPartsSteppedOn;
    if (m_NumberOfPartsSteppedOn >= m_LayerParts.size()) {
        if (!m_NeighborBellowTransform)
            CalculateLayerBellow();
        if (CanFall()) {
            StartFalling();
        }
    }
}

void dae::BurgerLayerComponent::Update() {
    if (m_IsFalling) {
        auto localPos = m_Transform->GetLocalPosition();
        localPos.y += m_FallingSpeed * TimeManager::GetInstance().GetDeltaTime();
        m_Transform->SetLocalPosition(localPos);
        for (auto const &layerPart : m_LayerParts)
            layerPart->SetPositionDirty();
        auto pos = m_Transform->GetWorldPosition();
        // if we're not the bottom layer
        if (m_NeighborBellowTransform) {
            auto otherPos = m_NeighborBellowTransform->GetWorldPosition();
            float smallMargin{2.f};
            if (!m_NeighborBellowComponent->IsFalling() && otherPos.y - pos.y <= m_LayerDimensions.y + smallMargin) {

                // if the layer bellow can not fall further, neither can we
                if (!m_NeighborBellowComponent->CanFall()) {
                    StopFalling();
                    return;
                } else
                    m_NeighborBellowComponent->StartFalling();
            }
        }

        float bowlOffset{55.f};
        if (m_LevelGrid->IsOnPlatform(pos, m_LayerDimensions) &&
            static_cast<int>(pos.y) % m_LevelGrid->GetGridSize() == static_cast<int>(m_LevelGrid->GetGridSize() / 1.9f))
            StopFalling();

        else if (m_LevelGrid->IsOnBurgerBowl({pos.x, pos.y - bowlOffset}, m_LayerDimensions))
            StopFalling();
    }
}

void dae::BurgerLayerComponent::StartFalling() {
    if (!m_NeighborBellowTransform)
        CalculateLayerBellow();
    m_IsFalling = true;
    m_NumberOfPartsSteppedOn = 0;

    // move ourselves down a bit and all the children up so the local y pos is back to 0
    {
        auto transform = GetOwner()->GetComponent<TransformComponent>();
        auto pos = transform->GetLocalPosition();
        pos.y += LayerPartComponent::SteppedOnOffset;
        transform->SetLocalPosition(pos);
    }

    for (auto const &layerPart : m_LayerParts) {
        auto component = layerPart->GetComponent<LayerPartComponent>();
        component->SetFallingState(true);
        if (!component->IsSteppedOn())
            continue;
        auto transform = layerPart->GetComponent<TransformComponent>();
        auto pos = transform->GetLocalPosition();
        pos.y -= LayerPartComponent::SteppedOnOffset;
        transform->SetLocalPosition(pos);
        layerPart->GetComponent<LayerPartComponent>()->SetFallingState(true);
    }
}

bool dae::BurgerLayerComponent::CanFall() {
    auto pos = m_Transform->GetWorldPosition();
    if (m_IsFalling)
        return true;
    // if we're not the bottom layer
    if (m_NeighborBellowTransform) {
        auto otherPos = m_NeighborBellowTransform->GetWorldPosition();
        float smallMargin{2.f};
        if (otherPos.y - pos.y <= m_LayerDimensions.y + smallMargin) {
            return m_NeighborBellowComponent->CanFall();
        }
    }

    if (m_LevelGrid->IsOnPlatform(pos, m_LayerDimensions) &&
        static_cast<int>(pos.y) % m_LevelGrid->GetGridSize() == static_cast<int>(m_LevelGrid->GetGridSize() / 1.9f))
        return true;

    if (m_LevelGrid->IsOnBurgerBowl(pos, m_LayerDimensions))
        return false;

    return false;
}

bool dae::BurgerLayerComponent::IsFalling() const {
    return m_IsFalling;
}

void dae::BurgerLayerComponent::StopFalling() {
    m_IsFalling = false;
    for (auto const &layerPart : m_LayerParts) {
        auto component = layerPart->GetComponent<LayerPartComponent>();
        component->SetFallingState(false);
    }
}

void dae::BurgerLayerComponent::CalculateLayerBellow() {
    auto pos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
    float closestDiff{1999999.f};
    for (auto const &layer : AllBurgerLayers) {
        // if it's this layer
        if (layer == GetOwner())
            continue;

        auto otherTransform = layer->GetComponent<TransformComponent>();
        auto otherPos = otherTransform->GetWorldPosition();
        // if it's not in the current row
        if (pos.x != otherPos.x)
            continue;
        // if it's above this layer
        if (otherPos.y < pos.y)
            continue;

        float currentDif{otherPos.y - pos.y};
        if (currentDif < closestDiff) {
            closestDiff = currentDif;
            m_NeighborBellowTransform = otherTransform;
            m_NeighborBellowComponent = layer->GetComponent<BurgerLayerComponent>();
        }
    }
}

void BurgerLayerComponent::CreateChildrenParts(std::vector<GameObject *> const &players, BurgerLayer layer) {

    // TODO fix tiles first with photoshop and hten the names here
    std::string filePath{"Data/Tiles/"};
    switch (layer) {
    case BurgerLayer::TopPaddy:
        filePath += "burgerpaddy_top";
        break;
    case BurgerLayer::Salad:
        filePath += "burgersalad";
        break;
    case BurgerLayer::Tomato:
        filePath += "burgertomato";
        break;
    case BurgerLayer::BottomPaddy:
        filePath += "burgerpaddy_bottom";
        break;
    }
    // create 4 parts of the current layer each one 35 pixels more the right of the previous one
    int originalWidth = 8;
    int originalHeight = 7;
    int scale = 3;
    int nrOfChildren = 4;

    m_LayerDimensions.x = static_cast<float>(originalWidth) * scale * nrOfChildren;
    m_LayerDimensions.y = static_cast<float>(originalHeight) * scale;

    auto go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_left.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 0, 0);
    m_LayerParts.emplace_back(go.get());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_middle_left.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 1, 0);
    m_LayerParts.emplace_back(go.get());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_middle_right.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 2, 0);
    m_LayerParts.emplace_back(go.get());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_right.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 3, 0);
    m_LayerParts.emplace_back(go.get());
    go.release();
}
