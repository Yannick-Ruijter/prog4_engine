#pragma once
#include "Component.hpp"

namespace dae {
    class TimeManager;
    class GameObject;
    class TextDisplay;
    class FpsDisplay : public Component {
      public:
        FpsDisplay(GameObject &owner);
        ~FpsDisplay() override = default;
        void Update() override;
        float GetDeltaTime() const;

      private:
        float m_LastFps{};
        TextDisplay *m_TextComponent{nullptr};
        TimeManager *m_TimeManager{nullptr};
    };
} // namespace dae
