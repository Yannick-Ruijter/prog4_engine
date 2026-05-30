#include "Binding.hpp"
#include "PlayerInput.hpp"
#include "Scene.hpp"
#include "assert.h"
#include <algorithm>
#include <iostream>
using namespace dae;

bool Scene::DebugOutputEnabled = false;

void Scene::Add(std::unique_ptr<GameObject> object)
{
    assert(object != nullptr && "Cannot add a null GameObject to the scene.");
    m_objects.emplace_back(std::move(object));
    if (DebugOutputEnabled)
        std::cout << "Added object number " << m_objects.size() << " to the scene\n";
}

void Scene::Remove(const GameObject &object)
{
    m_ToBeDeletedObjects.push_back(&object);
    // clang-format off
    if (DebugOutputEnabled)
        std::cout << "Marked object for removal: " << m_objects.size() - m_ToBeDeletedObjects.size() << " objects left\n";
    // clang-format on
}

void Scene::RemoveAll()
{
    m_objects.clear();
}

void dae::Scene::AddBinding(Binding *binding, PlayerInput *input)
{
    m_Bindings.emplace_back(binding, input);
    if (DebugOutputEnabled)
        std::cout << "Added binding: " << m_Bindings.size() << " in scene\n";
}

void Scene::Update()
{
    for (auto &object : m_objects)
    {
        object->Update();
    }
}

void Scene::LateUpdate()
{
    for (auto const objectPointer : m_ToBeDeletedObjects)
    {
        m_objects.erase(
            std::remove_if(
                m_objects.begin(), m_objects.end(),
                [objectPointer](const auto &ptr) { return ptr.get() == objectPointer; }),
            m_objects.end());
    }
    m_ToBeDeletedObjects.clear();

    for (auto &object : m_objects)
    {
        object->LateUpdate();
    }
}

void Scene::Render() const
{
    for (const auto &object : m_objects)
    {
        object->Render();
    }
}

dae::Scene::~Scene()
{
    if (DebugOutputEnabled)
        std::cout << "Unbinding all " << m_Bindings.size() << " bindings added to the scene...\n";
    for (auto const &[binding, input] : m_Bindings)
    {
        input->UnBind(binding);
    }
    if (DebugOutputEnabled)
        std::cout << "Unbinding done!\n";
}
