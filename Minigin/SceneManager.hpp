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
        Scene &CreateScene();

        void Update();
        void LateUpdate();
        void Render();

      private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene>> m_scenes{};
        Scene *m_CurrentScene{nullptr};
    };
} // namespace dae
