#pragma once

#include "Observer.hpp"
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
		Achievements() = default;
		void Notify(unsigned int eventId, GameObject* source);

	private:
		int m_ScoreThreshold{ 500 };
		bool m_FirstWinAchievementUnlocked{ false };
	};
}
