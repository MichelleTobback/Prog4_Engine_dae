#pragma once

#include "SteamAchievementSystem.h"

namespace dae
{
	enum class SpaceWarAchievementID
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	class SteamAchievementUtils final
	{
	public:
		static void CreateSpaceWarAchievements();
	};
}