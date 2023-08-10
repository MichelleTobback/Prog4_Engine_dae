#pragma once
#include "State/GameState/GameState.h"
#include "Components/BTTileGridComponent.h"

namespace dae
{
	class PlayerController;
	class BTTileGridComponent;
	class BTGameMode : public GameMode
	{
	public:
		BTGameMode(size_t level);
		virtual ~BTGameMode() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override = 0;
		virtual void OnExit() override;

		void OnSceneLoaded(Scene*, size_t index);
		void OnPlayerLostLife();
		void OnPlayerDeath();
		void AddIngredient();

		BTTileGridComponent& GetGrid() { return *m_pGrid; }
		GameObject* GetLevelRoot() { return m_pLevelRoot; }

		void RespawnAllActiveObjects();
		void RespawnAllActiveCharacters();
		void RespawnAll();
		void SpawnAllPlayers();
		void SpawnPlayer(size_t index);

		const std::vector<PlayerController*>& GetPlayers() const { return m_pPlayers; }
		PlayerController* GetPlayer(size_t index) const { return m_pPlayers[index]; }
		uint32_t GetPlayerMaxLifes() const { return m_PlayerMaxLifes; }
		uint32_t GetPlayerMaxPeppers() const { return m_PlayerMaxPeppers; }
		ObservableType<uint32_t>& GetScore() { return m_Score; }

	protected:
		virtual void StartRound() {}
		virtual GameObject* CreateHUD() { return nullptr; };
		void SetMaxPlayerLifes(uint32_t lifes) { m_PlayerMaxLifes = lifes; }
		void SetMaxPlayerPeppers(uint32_t peppers) { m_PlayerMaxPeppers = peppers; }
		void LoadLevels();
		void OpenNextLevel();

	private:
		void CreateGrid();

		GameObject* m_pLevelRoot{ nullptr };
		GameObject* m_pHUD{ nullptr };
		BTTileGridComponent* m_pGrid{nullptr};
		uint32_t m_PlayerMaxLifes{ 3 };
		uint32_t m_PlayerMaxPeppers{ 5 };
		uint32_t m_Ingredients{};
		uint32_t m_MaxIngredients{};
		static ObservableType<uint32_t> m_Score;
		std::vector<PlayerController*> m_pPlayers;
		std::vector<size_t> m_ScenesIndices;
		size_t m_CurrentLevel{};
	};
}