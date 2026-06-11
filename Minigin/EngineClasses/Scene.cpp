#include "Binding.hpp"
#include "PlayerInput.hpp"
#include "Scene.hpp"
#include "assert.h"
#include <algorithm>
#include <iostream>
using namespace dae;

bool Scene::DebugOutputEnabled = false;

void Scene::Add(std::unique_ptr<GameObject> object) {
  assert(object != nullptr && "Cannot add a null GameObject to the scene.");
  m_ToBeAddedObjects.emplace_back(std::move(object));
  if (DebugOutputEnabled)
    std::cout << "Added object number " << m_objects.size()
              << " to the scene\n";
}

void Scene::Remove(const GameObject &object) {
  m_ToBeDeletedObjects.push_back(&object);
  // clang-format off
    if (DebugOutputEnabled)
        std::cout << "Marked object for removal: " << m_objects.size() - m_ToBeDeletedObjects.size() << " objects left\n";
  // clang-format on
}

void Scene::RemoveAll() { m_objects.clear(); }

void Scene::Update() {
  for (auto &object : m_objects) {
    object->Update();
  }
}

void Scene::LateUpdate() {
  for (auto &object : m_objects) {
    object->LateUpdate();
  }

  for (auto const objectPointer : m_ToBeDeletedObjects) {
    m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
                                   [objectPointer](const auto &ptr) {
                                     return ptr.get() == objectPointer;
                                   }),
                    m_objects.end());
  }
  m_ToBeDeletedObjects.clear();

  for (auto &object : m_ToBeAddedObjects) {
    m_objects.push_back(std::move(object));
  }
  m_ToBeAddedObjects.clear();
}

void Scene::Render() const {
  for (const auto &object : m_objects) {
    object->Render();
  }
}

void dae::Scene::OnExit() {
  for (auto const &exitFunction : m_ExitFunctions)
    exitFunction();
}

void dae::Scene::OnEnter() {
  for (auto const &enterFunction : m_EnterFunctions)
    enterFunction();
}

void dae::Scene::AddEnterFunction(std::function<void()> const &onEnter) {
  m_EnterFunctions.push_back(onEnter);
}

void dae::Scene::AddExitFunction(std::function<void()> const &onExit) {
  m_ExitFunctions.push_back(onExit);
}

dae::Scene::~Scene() = default;
