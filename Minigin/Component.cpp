#include "Component.h"
#include "assert.h"

dae::Component::Component(GameObject& owner) :m_Owner(&owner) {}

dae::GameObject* dae::Component::GetOwner() const
{
    return m_Owner;
}

void dae::Component::Update() {}

void dae::Component::Render() const {}