#include "Achievements.hpp"
#include "GameObject.hpp"
#include "sdbm_hash.hpp"
#include "ScoreComponent.hpp"
using namespace dae;

void Achievements::Notify(unsigned int eventId, GameObject* source)
{
	if (eventId == "ScoreChanged"_h)
	{
		ScoreComponent* scoreComponent = source->GetComponent<ScoreComponent>();
		if (!m_FirstWinAchievementUnlocked && scoreComponent->GetScore() >= m_ScoreThreshold)
		{
			m_FirstWinAchievementUnlocked = true;
		}
	}
}