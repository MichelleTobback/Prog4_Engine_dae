#pragma once
#include "Observer.h"

#include "SteamIntegration.h"

namespace dae
{
	constexpr size_t MAX_ACHIEVEMENT_NAME_SIZE = 128;
	constexpr size_t MAX_ACHIEVEMENT_DESC_SIZE = 256;
	struct SteamAchievementDesc
	{
		int AchievementID;
		const char* AchievementIDTxt;
		char Name[MAX_ACHIEVEMENT_NAME_SIZE];
		char Description[MAX_ACHIEVEMENT_DESC_SIZE];
		bool Achieved;
		int IconImage;
	};

	class SteamAchievement : public Observer
	{
	public:
		SteamAchievement(const SteamAchievementDesc& desc);
		virtual ~SteamAchievement() override = default;

		virtual void Invoke(const Event& event) override;

	private:
		SteamAchievementDesc m_Data;

		void SetAchievement(const std::string& id);
	};
}