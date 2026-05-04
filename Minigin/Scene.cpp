#include "Scene.hpp"
#include "assert.h"
#include <algorithm>
using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
    assert(object != nullptr && "Cannot add a null GameObject to the scene.");
    m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject &object)
{
    m_ToBeDeletedObjects.push_back(&object);
}

void Scene::RemoveAll()
{
    m_objects.clear();
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
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
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
