#include "Component.h"
#include "assert.h"

dae::Component::Component(GameObject* parent) :m_Parent(parent) { assert(parent != nullptr); }

void dae::Component::Update(float deltaTime) { deltaTime; }

void dae::Component::Render() const {}