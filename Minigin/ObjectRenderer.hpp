#pragma once
#include "Component.hpp"
#include <memory>
#include <string>

namespace dae {
    class GameObject;
    class Transform;
    class Texture2DDisplay;
    class ObjectRenderer : public Component {
      public:
        ObjectRenderer(GameObject &owner);
        ~ObjectRenderer() override = default;
        void Update() override;
        void Render() const override;

      private:
        Transform *m_Transform;
        Texture2DDisplay *m_Texture2DDisplay;
    };
} // namespace dae
