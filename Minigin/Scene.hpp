#pragma once
#include "GameObject.hpp"
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace dae
{
    class Binding;
    class PlayerInput;
    class Scene final
    {
      public:
        void Add(std::unique_ptr<GameObject> object);
        void Remove(const GameObject &object);
        void RemoveAll();
        void AddBinding(Binding *binding, PlayerInput *input);

        void Update();
        void LateUpdate();
        void Render() const;

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
        std::vector<std::pair<Binding *, PlayerInput *>> m_Bindings;
    };

} // namespace dae
