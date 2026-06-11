#include "BurgerLayerPart.hpp"
#include "GameObject.hpp"
#include "LevelGrid.hpp"
#include "ObjectRenderer.hpp"
#include "RectCollider.hpp"
#include "ServiceProvider.hpp"
#include "SoundSystem.hpp"
#include "Subject.hpp"
#include "Texture2DDisplay.hpp"
#include "TimeManager.hpp"
#include "burgerLayer.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

std::vector<GameObject *> BurgerLayer::AllBurgerLayers = {};
BurgerLayer::BurgerLayer(GameObject &owner, BurgerLayerType layer,
                         LevelGrid *levelGrid)
    : Component(owner), m_LevelGrid{levelGrid},
      m_Transform{owner.GetComponent<Transform>()}, m_Type{layer} {
  AllBurgerLayers.emplace_back(GetOwner());
  CreateChildrenParts(layer);
}

void BurgerLayer::OnLayerPartCollided() {
  ++m_NumberOfPartsSteppedOn;
  if (m_NumberOfPartsSteppedOn >= m_LayerParts.size()) {
    if (!m_NeighborBellowTransform)
      CalculateLayerBellow();
    if (CanFall()) {
      StartFalling();
    }
  }
}

void dae::BurgerLayer::Update() {
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
      if (!m_NeighborBellowComponent->IsFalling() &&
          otherPos.y - pos.y <= m_LayerDimensions.y + smallMargin) {

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
        static_cast<int>(pos.y) % m_LevelGrid->GetGridSize() ==
            static_cast<int>(m_LevelGrid->GetGridSize() / 1.9f))
      StopFalling();

    else if (m_LevelGrid->IsOnBurgerBowl({pos.x, pos.y - bowlOffset},
                                         m_LayerDimensions))
      StopFalling();
  }
}

void dae::BurgerLayer::StartFalling() {
  ServiceProvider::GetSoundSystem().Play(4, 0.5f);
  if (!m_NeighborBellowTransform)
    CalculateLayerBellow();
  m_IsFalling = true;
  m_NumberOfPartsSteppedOn = 0;

  // move ourselves down a bit and all the children up so the local y pos is
  // back to 0
  {
    auto transform = GetOwner()->GetComponent<Transform>();
    auto pos = transform->GetLocalPosition();
    pos.y += BurgerLayerPart::SteppedOnOffset;
    transform->SetLocalPosition(pos);
  }

  for (auto const &layerPart : m_LayerParts) {
    auto component = layerPart->GetComponent<BurgerLayerPart>();
    component->SetFallingState(true);
    if (!component->IsSteppedOn())
      continue;
    auto transform = layerPart->GetComponent<Transform>();
    auto pos = transform->GetLocalPosition();
    pos.y -= BurgerLayerPart::SteppedOnOffset;
    transform->SetLocalPosition(pos);
    layerPart->GetComponent<BurgerLayerPart>()->SetFallingState(true);
  }

  m_BurgerFellEvent->NotifyObservers("StartedFalling"_h, GetOwner());
}

bool dae::BurgerLayer::CanFall() {
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
      static_cast<int>(pos.y) % m_LevelGrid->GetGridSize() ==
          static_cast<int>(m_LevelGrid->GetGridSize() / 1.9f))
    return true;

  if (m_LevelGrid->IsOnBurgerBowl(pos, m_LayerDimensions))
    return false;

  return false;
}

bool dae::BurgerLayer::IsFalling() const { return m_IsFalling; }

glm::vec2 dae::BurgerLayer::GetDimensions() const { return m_LayerDimensions; }

Subject *dae::BurgerLayer::BurgerFinishedEvent() const {
  return m_BurgerFinished.get();
}

Subject *dae::BurgerLayer::BurgerFellEvent() const {
  return m_BurgerFellEvent.get();
}

BurgerLayerType dae::BurgerLayer::Type() const { return m_Type; }

void dae::BurgerLayer::StopFalling() {
  ServiceProvider::GetSoundSystem().Play(5, 0.5f);
  m_IsFalling = false;
  for (auto const &layerPart : m_LayerParts) {
    auto component = layerPart->GetComponent<BurgerLayerPart>();
    component->SetFallingState(false);
  }
  m_BurgerFellEvent->NotifyObservers("BurgerFell"_h, GetOwner());
  if (m_Type == BurgerLayerType::TopPaddy && !CanFall())
    m_BurgerFinished->NotifyObservers("BurgerFinished"_h, GetOwner());
}

void dae::BurgerLayer::CalculateLayerBellow() {
  auto pos = GetOwner()->GetComponent<Transform>()->GetWorldPosition();
  float closestDiff{1999999.f};
  for (auto const &layer : AllBurgerLayers) {
    // if it's this layer
    if (layer == GetOwner())
      continue;

    auto otherTransform = layer->GetComponent<Transform>();
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
      m_NeighborBellowComponent = layer->GetComponent<BurgerLayer>();
    }
  }
}

void BurgerLayer::CreateChildrenParts(BurgerLayerType layer) {

  // TODO fix tiles first with photoshop and hten the names here
  std::string filePath{"Data/Tiles/"};
  switch (layer) {
  case BurgerLayerType::TopPaddy:
    filePath += "burgerpaddy_top";
    break;
  case BurgerLayerType::Salad:
    filePath += "burgersalad";
    break;
  case BurgerLayerType::Tomato:
    filePath += "burgertomato";
    break;
  case BurgerLayerType::BottomPaddy:
    filePath += "burgerpaddy_bottom";
    break;
  }
  // create 4 parts of the current layer each one 35 pixels more the right of
  // the previous one
  int originalWidth = 8;
  int originalHeight = 7;
  int scale = 3;
  int nrOfChildren = 4;

  m_LayerDimensions.x =
      static_cast<float>(originalWidth) * scale * nrOfChildren;
  m_LayerDimensions.y = static_cast<float>(originalHeight) * scale;

  auto createLayerPart = [&](const char *texture, int partNr) {
    auto go = std::make_unique<GameObject>();
    go->AddComponent<ObjectRenderer>();
    go->AddComponent<Texture2DDisplay>(
        filePath + texture, originalWidth * scale, originalHeight * scale);
    go->AddComponent<BurgerLayerPart>();
    go->AddComponent<RectCollider>(
        Rect{{}, {originalWidth * scale, originalHeight * scale}},
        LAYER_BURGERPART, LAYER_PEPPERGUY);
    go->SetParent(GetOwner(), false);
    go->GetComponent<Transform>()->SetLocalPosition(
        static_cast<float>(originalWidth) * scale * partNr, 0);
    auto layerPart{go->GetComponent<BurgerLayerPart>()};
    go->GetComponent<RectCollider>()->GetSubject()->AddObserver(layerPart);
    m_LayerParts.emplace_back(go.get());
    go.release();
  };
  createLayerPart("_left.png", 0);
  createLayerPart("_middle_left.png", 1);
  createLayerPart("_middle_right.png", 2);
  createLayerPart("_right.png", 3);
}
