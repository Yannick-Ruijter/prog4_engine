#include "Achievements.h"
#include "GameObject.h"
#include "Event.h"
#include "steam_api.h"
#include "ScoreComponent.h"
using namespace dae;

Achievements::Achievements()
	:m_Achievements{ _ACH_ID(static_cast<int>(SteamAchievements::ACH_WIN_ONE_GAME), "Winner") }
	,m_SteamAchievements{std::make_unique<CSteamAchievements>(m_Achievements.data(), static_cast<int>(m_Achievements.size()))}
{
}
void Achievements::Notify(Event event, GameObject* source)
{
	if (event == Event::ScoreChanged)
	{
		ScoreComponent* scoreComponent = source->GetComponent<ScoreComponent>();
		if (scoreComponent->GetScore() >= m_ScoreThreshold) 
			m_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
	}
}