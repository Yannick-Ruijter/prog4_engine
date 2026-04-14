#include "Achievements.h"
#include "GameObject.h"
#include "sdbm_hash.h"
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include "steam_api.h"
#pragma warning (pop)
#endif
#include "ScoreComponent.h"
using namespace dae;

Achievements::Achievements()
	:m_Achievements{ _ACH_ID(static_cast<int>(SteamAchievements::ACH_WIN_ONE_GAME), "Winner") }
	,m_SteamAchievements{std::make_unique<CSteamAchievements>(m_Achievements.data(), static_cast<int>(m_Achievements.size()))}
{
#if USE_STEAMWORKS
	SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &m_FirstWinAchievementUnlocked);
#endif
}
void Achievements::Notify(unsigned int eventId, GameObject* source)
{
	if (eventId == "ScoreChanged"_h)
	{
		ScoreComponent* scoreComponent = source->GetComponent<ScoreComponent>();
		if (!m_FirstWinAchievementUnlocked && scoreComponent->GetScore() >= m_ScoreThreshold)
		{
			m_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			m_FirstWinAchievementUnlocked = true;
		}
	}
}