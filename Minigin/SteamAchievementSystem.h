#pragma once
#include "Singleton.h"
#include "SteamAchievement.h"
#include <memory>
#include <map>

namespace dae
{
	class SteamAchievementSystem final : public Singleton<SteamAchievementSystem>
	{
	public:
		virtual ~SteamAchievementSystem() = default;

		SteamAchievement* Create(const SteamAchievementDesc& desc);
		SteamAchievement* GetAchievement(int index) { return m_pAchievements[index].get(); }

	private:

		std::map<int, std::unique_ptr<SteamAchievement>> m_pAchievements; //key = ID
	};
}