#pragma once
#include "Component.hpp"
#include "Observer.hpp"
#include <memory>
namespace dae {
    class Subject;
    class Score : public Component, public Observer {
      public:
        Score(GameObject &owner, std::unique_ptr<Subject> &&scoreChangedEvent = nullptr);
        void AddScore(int score);
        int GetScore() const;

        void Notify(EventId eventId, GameObject *source);
        Subject *GetSubject() const;

      private:
        std::unique_ptr<Subject> m_ScoreChangedEvent{nullptr};
        int m_CurrentScore{0};
    };
} // namespace dae
