#pragma once
#include "GameObject.hpp"
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace dae {
    class Binding;
    class PlayerInput;
    class Scene final {
      public:
        static bool DebugOutputEnabled;
        void Add(std::unique_ptr<GameObject> object);
        void Remove(const GameObject &object);
        void RemoveAll();

        void Update();
        void LateUpdate();
        void Render() const;
        void OnExit();
        void OnEnter();

        void AddEnterFunction(std::function<void()> const &onEnter);
        void AddExitFunction(std::function<void()> const &onExit);

        ~Scene();
        Scene(const Scene &other) = delete;
        Scene(Scene &&other) = delete;
        Scene &operator=(const Scene &other) = delete;
        Scene &operator=(Scene &&other) = delete;

      private:
        friend class SceneManager;
        explicit Scene() = default;

        std::vector<std::unique_ptr<GameObject>> m_objects{};
        std::vector<const GameObject *> m_ToBeDeletedObjects{};
        std::vector<std::function<void()>> m_EnterFunctions;
        std::vector<std::function<void()>> m_ExitFunctions;
    };

} // namespace dae
