#pragma once
#include "Component/Component.h"
#include "Core/Subject.h"
#include <memory>

namespace dae
{
	class PlayerScore;
	class IncreaseScore final : public Observer
	{
	public:
		IncreaseScore(PlayerScore* pScore);
		virtual ~IncreaseScore() = default;

		virtual void Invoke(const Event& event, Subject* pSubject) override;

	private:
		PlayerScore* m_pScore;
	};

	class Subject;
	class PlayerScore : public Component
	{
	public:
		PlayerScore(GameObject* pOwner);
		~PlayerScore() = default;

		void AddScore(int amount = 1);
		int GetScore() const { return m_Score; }

		Subject* GetOnScoreChanged() { return m_pOnScoreChanged.get(); }
		IncreaseScore* GetIncreaseScore() { return m_pIncreaseScore.get(); }

	private:
		int m_Score{ 0 };
		std::unique_ptr<Subject> m_pOnScoreChanged{ nullptr };
		std::unique_ptr<Subject> m_pOnPlayerWin{ nullptr };

		std::unique_ptr<IncreaseScore> m_pIncreaseScore{ nullptr };
	};
}