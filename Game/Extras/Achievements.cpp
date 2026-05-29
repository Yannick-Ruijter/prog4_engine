#include "Achievements.hpp"
#include "GameObject.hpp"
#include "ScoreComponent.hpp"
#include "sdbm_hash.hpp"
using namespace dae;

void Achievements::Notify(EventId eventId, GameObject *source)
{
    if (eventId == "ScoreChanged"_h)
    {
        ScoreComponent *scoreComponent = source->GetComponent<ScoreComponent>();
        if (!m_FirstWinAchievementUnlocked && scoreComponent->GetScore() >= m_ScoreThreshold)
        {
            m_FirstWinAchievementUnlocked = true;
        }
    }
}
