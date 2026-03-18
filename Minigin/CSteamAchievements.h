#pragma once
#include <cstdint>
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include "steam_api.h"
#pragma warning (pop)
#endif

namespace dae {
	struct Achievement_t
	{
		int m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

	class CSteamAchievements
	{
	private:
		std::uint64_t m_iAppID; // Our current AppID
		Achievement_t* m_pAchievements; // Achievements data
		int m_iNumAchievements; // The number of Achievements
		bool m_bInitialized; // Are we ready to use the API?

	public:
		CSteamAchievements(Achievement_t* Achievements, int NumAchievements);
		~CSteamAchievements();

		bool Initialize();
		bool SetAchievement(const char* ID);
#if USE_STEAMWORKS
		STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);
#endif
	};
}