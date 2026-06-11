#pragma once
#include "Component.hpp"
#include <glm/glm.hpp>
namespace dae {
class GameObject;
class Transform : public Component {
public:
  Transform(GameObject &owner, glm::vec2 const &position = {0, 0});
  virtual ~Transform() = default;
  virtual void Update() override;
  const glm::vec2 &GetWorldPosition();
  const glm::vec2 &GetLocalPosition() const { return m_LocalPosition; }
  void SetLocalPosition(float x, float y);
  void SetLocalPosition(const glm::vec2 &position);
  void SetWorldPositionDirty() { m_UpdateWorldPos = true; }

private:
  glm::vec2 m_LocalPosition{};
  glm::vec2 m_WorldPosition{};
  bool m_UpdateWorldPos{};
};
} // namespace dae
