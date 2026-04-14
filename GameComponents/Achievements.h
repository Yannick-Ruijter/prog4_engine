#pragma once

#include "Observer.h"
#include "CSteamAchievements.h"
#include <memory>
#include <vector>

namespace dae
{
	enum class SteamAchievements
	{
		ACH_WIN_ONE_GAME = 0,
	};
	class Achievements : public Observer
	{
	public:
		Achievements();
		void Notify(unsigned int eventId, GameObject* source);

	private:
		std::vector<Achievement_t> m_Achievements{};
		std::unique_ptr<CSteamAchievements> m_SteamAchievements{ nullptr };

		int m_ScoreThreshold{ 500 };
		bool m_FirstWinAchievementUnlocked{ false };
	};
}
