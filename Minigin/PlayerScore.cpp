#include "PlayerScore.h"
#include "Subject.h"

#include "SteamAchievementUtils.h"

dae::PlayerScore::PlayerScore(GameObject* pOwner)
	: Component{pOwner}
{
	m_pOnScoreChanged = std::make_unique<Subject>();
	m_pIncreaseScore = std::make_unique<IncreaseScore>(this);
	m_pOnPlayerWin = std::make_unique<Subject>();
	m_pOnPlayerWin->AddObserver(
		SteamAchievementSystem::GetInstance().GetAchievement(
			static_cast<int>(SpaceWarAchievementID::ACH_WIN_ONE_GAME)));
}

void dae::PlayerScore::AddScore(int amount)
{
	m_Score += amount;
	Event e{};
	e.data = reinterpret_cast<char*>(&m_Score);
	e.dataSize = sizeof(int);
	m_pOnScoreChanged->Invoke(e);

	const int maxScore{ 500 };
	if (m_Score >= maxScore)
	{
		m_pOnPlayerWin->Invoke(e);
	}
}

dae::IncreaseScore::IncreaseScore(PlayerScore* pScore)
	: Observer{}
	, m_pScore{pScore}
{
	
}

void dae::IncreaseScore::Invoke(const Event&)
{
	m_pScore->AddScore();
}
