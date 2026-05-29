#pragma once
#include "Component.hpp"
#include "Observer.hpp"

namespace dae
{
    class HealthComponent;
    class TextComponent;
    enum class Event;
    class LivesDisplayComponent : public Component, public Observer
    {
      public:
        LivesDisplayComponent(GameObject &gameObject, HealthComponent &targetHealth);

        void Notify(EventId eventId, GameObject *source) override;

      private:
        HealthComponent *m_TargetHealth{nullptr};
        TextComponent *m_TextComponent{nullptr};
    };
} // namespace dae
