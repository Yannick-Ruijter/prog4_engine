#include "SceneManager.hpp"
#include "Scene.hpp"

void dae::SceneManager::Update()
{
    m_CurrentScene->Update();
}

void dae::SceneManager::LateUpdate()
{
    m_CurrentScene->LateUpdate();
}

void dae::SceneManager::Render()
{
    m_CurrentScene->Render();
}

void dae::SceneManager::SetActiveScene(Scene *scene)
{
    // check if the scene is in the added scenes
    if (std::find_if(m_Scenes.begin(),
                     m_Scenes.end(),
                     [&](auto &other)
                     { return other.get() == scene; }) != m_Scenes.end())
        m_CurrentScene = scene;
}

dae::Scene &dae::SceneManager::CreateScene()
{
    m_Scenes.emplace_back(new Scene());
    return *m_Scenes.back();
}
