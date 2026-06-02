#pragma once
#include "Component.hpp"
#include "Observer.hpp"

namespace dae {
    class Health;
    class TextDisplay;
    enum class Event;
    class LivesDisplay : public Component, public Observer {
      public:
        LivesDisplay(GameObject &gameObject, Health &targetHealth);

        void Notify(EventId eventId, GameObject *source) override;

      private:
        Health *m_TargetHealth{nullptr};
        TextDisplay *m_TextComponent{nullptr};
    };
} // namespace dae
