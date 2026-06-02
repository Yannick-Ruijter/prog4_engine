#pragma once

#include "Observer.hpp"

enum class Event;
namespace dae
{
    class GameObject;
    class TextDisplay;
    class PlayerLivesObserver : public Observer
    {
      public:
        PlayerLivesObserver(TextDisplay *textComponent);
        ~PlayerLivesObserver() override = default;
        void Notify(EventId eventId, GameObject *source) override;

      private:
        TextDisplay *m_TargetTextComponent{nullptr};
    };
} // namespace dae
