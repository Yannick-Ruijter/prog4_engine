#include "GameObject.hpp"
#include "RectCollider.hpp"
#include "Renderer.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
#include <algorithm>
#include <bit>

using namespace dae;
std::vector<RectCollider *> RectCollider::m_Colliders = {};
RectCollider::RectCollider(GameObject &owner, Rect rect, int32_t layer,
                           int32_t layerMask)
    : Component(owner), m_Rect{rect}, m_Layer{layer}, m_LayerMask{layerMask},
      m_Subject{std::make_unique<Subject>()} {
  m_Colliders.push_back(this);
}

dae::RectCollider::~RectCollider() { std::erase(m_Colliders, this); }

const Rect &dae::RectCollider::GetRect() const { return m_Rect; }

void dae::RectCollider::Update() {
  m_Rect.topLeft = GetOwner()->GetWorldPosition();
}

void dae::RectCollider::Render() const {
  SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0,
                         255);
  SDL_RenderRect(Renderer::GetInstance().GetSDLRenderer(),
                 std::bit_cast<SDL_FRect *>(&m_Rect));
}

void dae::RectCollider::LateUpdate() {
  if (m_LayerMask == LAYER_NONE)
    return;
  for (auto collider : m_Colliders) {
    // if it's either this collider or not part of our mask

    if (collider == this ||
        (collider->GetLayer() & m_LayerMask) != collider->GetLayer())
      continue;

    auto otherRect = collider->GetRect();

    if ((otherRect.topLeft.x + otherRect.dimensions.x) < m_Rect.topLeft.x ||
        otherRect.topLeft.x > (m_Rect.topLeft.x + m_Rect.dimensions.x))
      continue;
    if ((otherRect.topLeft.y + otherRect.dimensions.y) < m_Rect.topLeft.y ||
        otherRect.topLeft.y > (m_Rect.topLeft.y + m_Rect.dimensions.y))
      continue;

    m_LastCollision = collider;
    m_Subject->NotifyObservers("OnCollision"_h, GetOwner());
  }
}

RectCollider *dae::RectCollider::GetLastCollision() const {
  return m_LastCollision;
}

int32_t dae::RectCollider::GetLayer() const { return m_Layer; }

Subject *dae::RectCollider::GetSubject() const { return m_Subject.get(); }
