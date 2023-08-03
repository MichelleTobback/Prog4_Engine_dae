#pragma once
#include "State/GameState/GameState.h"
#include "Components/BTTileGridComponent.h"

namespace dae
{
	class BTTileGridComponent;
	class BTGameMode : public GameMode
	{
	public:
		virtual ~BTGameMode() = default;

		virtual void OnEnter() = 0;
		virtual StatePtr OnUpdate() = 0;
		virtual void OnExit() = 0;

		void OnSceneLoaded();

		BTTileGridComponent& GetGrid() { return *m_pGrid; }
		GameObject* GetLevelRoot() { return m_pLevelRoot; }

		void RespawnAllActiveObjects();
		void RespawnAll();
		void SpawnPlayer();

		uint32_t GetPlayerMaxLifes() const { return m_PlayerMaxLifes; }
		uint32_t GetPlayerMaxPeppers() const { return m_PlayerMaxPeppers; }
		ObservableType<uint32_t>& GetScore() { return m_Score; }

	protected:
		virtual GameObject* CreateHUD() { return nullptr; };
		void SetMaxPlayerLifes(uint32_t lifes) { m_PlayerMaxLifes = lifes; }
		void SetMaxPlayerPeppers(uint32_t peppers) { m_PlayerMaxPeppers = peppers; }

	private:
		void CreateGrid();

		GameObject* m_pLevelRoot{ nullptr };
		GameObject* m_pHUD{ nullptr };
		BTTileGridComponent* m_pGrid{nullptr};
		uint32_t m_PlayerMaxLifes{ 3 };
		uint32_t m_PlayerMaxPeppers{ 5 };
		ObservableType<uint32_t> m_Score{};
	};
}