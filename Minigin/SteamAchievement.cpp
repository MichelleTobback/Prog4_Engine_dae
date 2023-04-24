#include "SteamAchievement.h"

dae::SteamAchievement::SteamAchievement(const SteamAchievementDesc& desc)
	: m_Data{desc}
{
	
}

void dae::SteamAchievement::Invoke(const Event&)
{
	if (!m_Data.Achieved)
		SetAchievement(m_Data.AchievementIDTxt);
}

void dae::SteamAchievement::SetAchievement(const std::string& id)
{
	if (SteamIntegration::GetInstance().IsActive())
	{
		SteamUserStats()->SetAchievement(id.c_str());
		SteamUserStats()->StoreStats();
		m_Data.Achieved = true;
	}
}
