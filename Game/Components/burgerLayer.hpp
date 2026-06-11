#pragma once
#include "Component.hpp"
#include "Subject.hpp"
namespace dae {
class BurgerLayerPart;
class LevelGrid;
enum class BurgerLayerType { TopPaddy, Salad, Tomato, BottomPaddy };
class BurgerLayer : public Component {
public:
  BurgerLayer(GameObject &owner, BurgerLayerType layer, LevelGrid *levelGrid);
  virtual ~BurgerLayer();
  void OnLayerPartCollided();
  virtual void Update() override;
  void StartFalling();
  bool CanFall();
  bool IsFalling() const;
  glm::vec2 GetDimensions() const;
  Subject *BurgerFinishedEvent() const;
  Subject *BurgerFellEvent() const;
  BurgerLayerType Type() const;
  static std::vector<GameObject *> AllBurgerLayers;

private:
  void StopFalling();
  void CalculateLayerBellow();
  bool m_IsFalling{false};
  void CreateChildrenParts(BurgerLayerType layer);
  std::vector<GameObject *> m_LayerParts;
  glm::vec2 m_LayerDimensions{};
  uint32_t m_NumberOfPartsSteppedOn{0};
  LevelGrid *m_LevelGrid;
  Transform *m_Transform{nullptr};
  Transform *m_NeighborBellowTransform{nullptr};
  BurgerLayer *m_NeighborBellowComponent{nullptr};
  float m_FallingSpeed{60.f};
  std::unique_ptr<Subject> m_BurgerFellEvent{std::make_unique<Subject>()};
  std::unique_ptr<Subject> m_BurgerFinished{std::make_unique<Subject>()};
  BurgerLayerType m_Type{};
};
} // namespace dae
