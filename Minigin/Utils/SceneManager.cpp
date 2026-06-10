#include "Scene.hpp"
#include "SceneManager.hpp"

void dae::SceneManager::Update() {
  if (m_FutureScene) {
    SetActiveScene(m_FutureScene);
    m_FutureScene = nullptr;
  }
  m_CurrentScene->Update();
}

void dae::SceneManager::LateUpdate() { m_CurrentScene->LateUpdate(); }

void dae::SceneManager::Render() { m_CurrentScene->Render(); }

void dae::SceneManager::UnLoadScene(Scene *scene) {
  // erase ALL elements that return true
  std::erase_if(m_Scenes, [&](auto &other) { return other.get() == scene; });
}

void dae::SceneManager::SetActiveScene(Scene *scene) {
  // check if the scene is in the added scenes
  if (m_CurrentScene)
    m_CurrentScene->OnExit();
  if (std::find_if(m_Scenes.begin(), m_Scenes.end(), [&](auto &other) {
        return other.get() == scene;
      }) != m_Scenes.end())
    m_CurrentScene = scene;

  if (scene)
    scene->OnEnter();
}

dae::Scene *dae::SceneManager::CreateScene() {
  m_Scenes.emplace_back(new Scene());
  return m_Scenes.back().get();
}

dae::Scene *dae::SceneManager::GetActiveScene() const { return m_CurrentScene; }
