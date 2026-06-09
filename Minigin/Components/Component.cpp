#include "Component.hpp"
#include "GameObject.hpp"
#include "assert.h"

dae::Component::Component(GameObject &owner) : m_Owner(&owner) {}

dae::GameObject *dae::Component::GetOwner() const { return m_Owner; }

void dae::Component::Update() {}

void dae::Component::LateUpdate() {}

void dae::Component::Render() const {}

template <typename T> T *dae::Component::GetComponent() {
  return m_Owner->template GetComponent<T>();
}
