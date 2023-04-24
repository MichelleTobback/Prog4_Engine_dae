#include "SteamAchievementSystem.h"

dae::SteamAchievement* dae::SteamAchievementSystem::Create(const SteamAchievementDesc& desc)
{
	m_pAchievements[desc.AchievementID] = std::make_unique<SteamAchievement>(desc);
	return m_pAchievements[desc.AchievementID].get();
}
