#pragma once
#include "Component.hpp"
#include "Subject.hpp"
#include <glm/glm.hpp>
#include <vector>
namespace dae
{
struct Rect
{
    glm::vec2 topLeft;
    glm::vec2 dimensions;
};

class RectColliderComponent : public Component, public Subject
{
  public:
    RectColliderComponent(GameObject &owner);
    const Rect &GetRect() const;
    void Update();
    RectColliderComponent *GetLastCollision() const;

  private:
    Rect m_Rect{};
    RectColliderComponent *m_LastCollision{nullptr};
    static std::vector<RectColliderComponent *> m_Colliders;
};
} // namespace dae
