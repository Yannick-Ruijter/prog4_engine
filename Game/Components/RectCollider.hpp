#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>
#include <vector>
// clang-format off
#define LAYER_NONE 0b0
#define LAYER_BURGER 0b1
#define LAYER_ENEMY 0b10
#define LAYER_PEPPERGUY 0b100
#define LAYER_PEPPER 0b1000
#define LAYER_BURGERPART 0b10000
// clang-format on
namespace dae {
struct Rect {
  glm::vec2 topLeft;
  glm::vec2 dimensions;
};
class Subject;
class RectCollider : public Component {
public:
  RectCollider(GameObject &owner, Rect rect, int32_t layer,
               int32_t layerMask = 0b11111111'11111111'11111111'11111111);
  virtual ~RectCollider() override;
  const Rect &GetRect() const;
  void Update() override;
  void Render() const override;
  void LateUpdate() override;
  RectCollider *GetLastCollision() const;
  int32_t GetLayer() const;
  Subject *GetSubject() const;
  bool IsActive() const;
  void SetActiveState(bool isActive);

private:
  Rect m_Rect{};
  RectCollider *m_LastCollision{nullptr};
  static std::vector<RectCollider *> m_Colliders;
  int32_t m_Layer;
  int32_t m_LayerMask;
  std::unique_ptr<Subject> m_Subject;
  bool m_IsActive{true};
};
} // namespace dae
