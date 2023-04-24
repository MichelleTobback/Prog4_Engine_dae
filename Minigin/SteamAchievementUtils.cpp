#include "SteamAchievementUtils.h" 

void dae::SteamAchievementUtils::CreateSpaceWarAchievements()
{
	SteamAchievementDesc winnerDesc{};
	winnerDesc.AchievementIDTxt = "ACH_WIN_ONE_GAME";
	winnerDesc.AchievementID = static_cast<int>(SpaceWarAchievementID::ACH_WIN_ONE_GAME);
	winnerDesc.Achieved = false;
	SteamAchievementSystem::GetInstance().Create(winnerDesc);
}
