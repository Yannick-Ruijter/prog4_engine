#pragma once
#include <memory>

namespace dae
{
    class Scene;
    class SceneLoader
    {
      public:
        virtual ~SceneLoader() = default;
        virtual Scene *AcquireScene() const = 0;
    };
} // namespace dae
