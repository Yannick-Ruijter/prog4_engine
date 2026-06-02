#include "Score.hpp"
#include "Subject.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

dae::Score::Score(GameObject &owner, std::unique_ptr<Subject> &&scoreChangedEvent)
    : Component(owner), m_ScoreChangedEvent{std::move(scoreChangedEvent)}
{
}

void Score::AddScore(int score)
{
    m_CurrentScore += score;
    m_ScoreChangedEvent->NotifyObservers("ScoreChanged"_h, GetOwner());
}

int dae::Score::GetScore() const
{
    return m_CurrentScore;
}

void dae::Score::Notify(EventId eventId, GameObject *)
{
    if (eventId == "ItemPickedUp"_h)
        AddScore(100);
}

Subject *dae::Score::GetSubject() const
{
    return m_ScoreChangedEvent.get();
}
