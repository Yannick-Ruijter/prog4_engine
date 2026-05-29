#pragma once
#include "Scene.hpp"
#include "Singleton.hpp"
#include <memory>
#include <string>
#include <vector>

// restrain the type T to always have the function LoadScene that returns a scene*
template <typename T, typename... Args>
concept IsSceneLoader = requires(Args &&...args) {
    { T::LoadScene(std::forward<Args>(args)...) } -> std::same_as<dae::Scene *>;
};

namespace dae
{
    class Scene;
    class SceneLoader;
    class SceneManager final : public Singleton<SceneManager>
    {
      public:
        Scene *CreateScene();

        void Update();
        void LateUpdate();
        void Render();

        void UnLoadScene(Scene *scene);

        template <typename Loader, typename... Args>
            requires IsSceneLoader<Loader, Args...>
        Scene *LoadScene(bool setAsActive, Args &&...args)
        {
            auto scene = Loader::LoadScene(std::forward<Args>(args)...);
            if (setAsActive)
                SetActiveScene(scene);
            return scene;
        }

        void SetActiveScene(Scene *scene);

      private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::unique_ptr<Scene>> m_Scenes{};
        Scene *m_CurrentScene{nullptr};
    };
} // namespace dae
