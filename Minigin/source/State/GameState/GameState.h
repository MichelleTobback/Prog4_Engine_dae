#pragma once
#include "Core/Singleton.h"
#include "State/StateMachine.h"
#include "GameMode.h"

#include <memory>

namespace dae
{
	class GameState final : public Singleton<GameState>
	{
	public:
		~GameState() = default;

		void Start();
		void Update();
		void Shutdown();

		void SetGameMode(std::shared_ptr<GameMode>&& pGameState);
		GameMode& GetGameMode() const;

	protected:
		GameState();

	private:
		friend class Singleton<GameState>;
		std::unique_ptr<StateMachine> m_pGameModeFSM{nullptr};
		std::shared_ptr<GameMode> m_pEntryGameMode;
		bool m_Running{ false };
	};

	class DefaultGameState final : public State
	{
	public:
		DefaultGameState() = default;
		~DefaultGameState() = default;

	protected:
		virtual void OnEnter() override {}
		virtual StatePtr OnUpdate() override { return nullptr; }
		virtual void OnExit() override {}
	};
}