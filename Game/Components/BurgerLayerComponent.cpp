#include "GameObject.hpp"
#include "LayerPartComponent.hpp"
#include "RenderComponent.hpp"
#include "Texture2DComponent.hpp"
#include "burgerLayerComponent.hpp"
using namespace dae;

BurgerLayerComponent::BurgerLayerComponent(
    GameObject &owner, BurgerLayer /*layer*/, std::vector<GameObject *> const &players)
    : Component(owner) {
    CreateChildrenParts(players);
}

void BurgerLayerComponent::OnLayerPartCollided() {
    ++m_NumberOfPartsSteppedOn;
    if (m_NumberOfPartsSteppedOn >= m_LayerParts.size()) {
        for (auto const &part : m_LayerParts)
            part->Startfalling();
    }
}

void BurgerLayerComponent::CreateChildrenParts(std::vector<GameObject *> const &players) {

    // create 4 parts of the current layer each one 35 pixels more the right of the previous one
    auto go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>("Data/Tiles/burgercheese_left.png", 35, 30);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(0, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>("Data/Tiles/burgercheese_middle_left.png", 35, 30);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(35, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>("Data/Tiles/burgercheese_middle_right.png", 35, 30);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(70, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();

    go = std::make_unique<GameObject>();
    go->AddComponent<RenderComponent>();
    go->AddComponent<Texture2DComponent>("Data/Tiles/burgercheese_right.png", 35, 30);
    go->AddComponent<LayerPartComponent>(players);
    go->SetParent(GetOwner(), false);
    go->GetComponent<TransformComponent>()->SetLocalPosition(105, 0);
    m_LayerParts.emplace_back(go->GetComponent<LayerPartComponent>());
    go.release();
}
