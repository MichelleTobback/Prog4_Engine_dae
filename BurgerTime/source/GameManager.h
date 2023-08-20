#pragma once
#include "Core/Singleton.h"
#include "States/StackedStateMachine.h"
#include "States/GameStates/GameState.h"
#include "Core/Delegate.h"

#include "HighScoreSerializer.h"

namespace dae
{
	class GameManager final : public Singleton<GameManager>
	{
	public:
		GameManager();
		~GameManager();

		GameState& GetState() const;
		void PushState(const std::shared_ptr<GameState>& pState);
		void PopState(int amount = 1);
		void SetState(const std::shared_ptr<GameState>& pState);
		void PopWhile(const std::function<bool(size_t)>& fn);

		void SetPlayerName(const std::string& name);
		const std::string& GetPlayerName();
		ObservableType<uint32_t>& GetHiScore() { return m_HiScore; }

		void UpdateHiScores(uint32_t score);
		const GameSaveData& GetSaveData() const { return m_SaveData; }

	private:
		std::unique_ptr<StackedStateMachine<GameState, Scene&>> m_pStateMachine;
		std::stack<size_t> m_PrevScenes;

		GameSaveData m_SaveData{};

		std::string m_PlayerName{};
		ObservableType<uint32_t> m_HiScore{};
	};
}