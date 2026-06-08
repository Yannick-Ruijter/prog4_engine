#pragma once
#include "Component.hpp"
#include "Subject.hpp"
#include <glm/glm.hpp>
#include <vector>
#define LAYER_BURGER 0b1
#define LAYER_ENEMY 0b10
namespace dae {
struct Rect {
  glm::vec2 topLeft;
  glm::vec2 dimensions;
};

class RectCollider : public Component {
public:
  RectCollider(GameObject &owner, Rect rect, uint32_t layer = 0,
               uint32_t layerMask = 0b11111111'11111111'11111111'11111111);
  virtual ~RectCollider() override;
  const Rect &GetRect() const;
  void Update() override;
  void Render() const override;
  void LateUpdate() override;
  RectCollider *GetLastCollision() const;
  uint32_t GetLayer() const;
  Subject *GetSubject() const;

private:
  std::unique_ptr<Subject> m_Subject;
  Rect m_Rect{};
  RectCollider *m_LastCollision{nullptr};
  static std::vector<RectCollider *> m_Colliders;
  uint32_t m_Layer;
  uint32_t m_LayerMask;
};
} // namespace dae
