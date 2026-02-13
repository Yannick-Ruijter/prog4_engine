#include <algorithm>
#include "Scene.h"
#include "assert.h"
using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_ToBeDeletedObjects.push_back(&object);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		if (object->HasFpsComponent())
			object->SetRenderText(std::to_string(object->GetFps()));
		object->Update(deltaTime);
	}
}

void Scene::LateUpdate()
{
	for (auto const objectPoint : m_ToBeDeletedObjects)
	{
		m_objects.erase(
			std::remove_if(
				m_objects.begin(),
				m_objects.end(),
				[objectPoint](const auto& ptr) { return ptr.get() == objectPoint; }
			),
			m_objects.end()
		);
	}
	m_ToBeDeletedObjects.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

