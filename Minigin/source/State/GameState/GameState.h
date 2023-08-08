#pragma once
#include "Core/Singleton.h"
#include "State/StateMachine.h"

#include <memory>

namespace dae
{
	using GameMode = State;
	class GameState final : public Singleton<GameState>
	{
	public:
		~GameState() = default;

		void Start();
		void Update();
		void Shutdown();

		void SetGameMode(std::shared_ptr<GameMode>&& pGameMode);
		GameMode& GetGameMode() const;

	protected:
		GameState();

	private:
		friend class Singleton<GameState>;
		std::unique_ptr<StateMachine> m_pGameModeFSM{nullptr};
		std::shared_ptr<GameMode> m_pEntryGameMode;
		bool m_Running{ false };
	};

	class DefaultGameMode final : public GameMode
	{
	public:
		DefaultGameMode() = default;
		virtual ~DefaultGameMode() override = default;

		DefaultGameMode(const DefaultGameMode& other) = delete;
		DefaultGameMode(DefaultGameMode&& other) = delete;
		DefaultGameMode& operator=(const DefaultGameMode& other) = delete;
		DefaultGameMode& operator=(DefaultGameMode&& other) = delete;

		virtual void OnEnter() override {}
		virtual StatePtr OnUpdate() override { return this; }
		virtual void OnExit() override {}
	};
}