#pragma once
#include "States/Enemy/EnemyState.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class BTGameMode;
	class NodeComponent;
	class CharacterInfoComponent;
	class EnemyGoToPlayerState final : public EnemyState
	{
	public:
		EnemyGoToPlayerState(EnemyComponent* pEnemy);
		virtual ~EnemyGoToPlayerState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

	private:
		NodeComponent* GetNode(const glm::vec3& pos);
		void CalculatePath();
		bool IsOnLadder();
		bool PlayerMoved();
		
		size_t m_NextNodeIndex{};
		std::vector<NodeComponent*> m_pPath;
		glm::vec3 m_PlayerPos{};

		BTGameMode* m_pCurrentGameMode{ nullptr };
	};
}