#pragma once
#include "Component.hpp"
#include "Observer.hpp"

namespace dae {
    class Score;
    class TextDisplay;
    enum class Event;
    class ScoreDisplay : public Component, public Observer {
      public:
        ScoreDisplay(GameObject &gameObject, Score &targetHealth);

        void Notify(EventId eventId, GameObject *source) override;

      private:
        Score *m_TargetScore{nullptr};
        TextDisplay *m_TextComponent{nullptr};
    };
} // namespace dae
