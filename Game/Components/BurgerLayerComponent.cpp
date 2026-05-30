#include "GameObject.hpp"
#include "LayerPartComponent.hpp"
#include "RenderComponent.hpp"
#include "Texture2DComponent.hpp"
#include "burgerLayerComponent.hpp"
using namespace dae;

BurgerLayerComponent::BurgerLayerComponent(
    GameObject &owner, BurgerLayer layer, std::vector<GameObject *> const &players)
    : Component(owner) {
    CreateChildrenParts(players, layer);
}

void BurgerLayerComponent::OnLayerPartCollided() {
    ++m_NumberOfPartsSteppedOn;
    if (m_NumberOfPartsSteppedOn >= m_LayerParts.size()) {
        for (auto const &part : m_LayerParts)
            part->Startfalling();
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
    auto go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_left.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 0, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_middle_left.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 1, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_middle_right.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 2, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>(filePath + "_right.png", originalWidth * scale, originalHeight * scale);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(static_cast<float>(originalWidth) * scale * 3, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();
}
