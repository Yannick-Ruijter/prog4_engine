#pragma once
#include "Scene.hpp"
#include "Singleton.hpp"
#include <memory>
#include <string>
#include <vector>

namespace dae
{
    class Scene;
    class SceneManager final : public Singleton<SceneManager>
    {
      public:
        Scene *CreateScene();

        void Update();
        void LateUpdate();
        void Render();

        void UnLoadScene(Scene *scene);
        void LoadScene(Scene *Scene);
        void SetActiveScene(Scene *scene);

      private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene>> m_Scenes{};
        Scene *m_CurrentScene{nullptr};
    };
} // namespace dae
