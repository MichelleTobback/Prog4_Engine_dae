#pragma once
#include "States/GameStates/GameState.h"
#include "Components/BTTileGridComponent.h"

namespace dae
{
	class CameraComponent;
	class PlayerController;
	class BTGameMode : public GameState
	{
	public:
		BTGameMode(size_t lvlIdx);
		virtual ~BTGameMode() = default;

		virtual void OnEnter(Scene& scene) override;
		virtual void OnExit(Scene& scene) override;

		virtual void OnSceneLoaded(Scene*, size_t index);
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

		void RespawnAfterDuration(GameObject* pObject, float duration);

		const std::vector<PlayerController*>& GetPlayers() const { return m_pPlayers; }
		PlayerController* GetPlayer(size_t index) const { return m_pPlayers[index]; }
		uint32_t GetPlayerMaxLifes() const { return m_PlayerMaxLifes; }
		uint32_t GetPlayerMaxPeppers() const { return m_PlayerMaxPeppers; }
		ObservableType<uint32_t>& GetScore() { return m_Score; }
		glm::vec3 GetClosestPlayerPos(const glm::vec3& pos) const;
		void OpenNextLevel();

	protected:
		virtual void StartRound() {}
		virtual GameObject* CreateHUD(Scene&) { return nullptr; };
		virtual GameObject* CreatePlayer(dae::Scene* pScene);
		std::vector<BTTile*>& GetPlayerSpawns() { return m_pPlayerSpawns; }
		void SetMaxPlayerLifes(uint32_t lifes) { m_PlayerMaxLifes = lifes; }
		void SetMaxPlayerPeppers(uint32_t peppers) { m_PlayerMaxPeppers = peppers; }
		void LoadLevels();

	private:
		void CreateGrid(Scene& scene);

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
		std::vector<BTTile*> m_pPlayerSpawns;
		size_t m_CurrentLevel{};
		CameraComponent* m_pCamera{ nullptr };

		unsigned short m_MusicId{};
	};
}