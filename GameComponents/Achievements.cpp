#include "Achievements.h"
#include "GameObject.h"
#include "sdbm_hash.h"
#undef USE_STEAMWORKS
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include "steam_api.h"
#pragma warning (pop)
#endif
#include "ScoreComponent.h"
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