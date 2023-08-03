#pragma once
#include "States/CharacterState.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class NodeComponent;
	class EnemyGoToPlayerState final : public CharacterState
	{
	public:
		EnemyGoToPlayerState(CharacterInfoComponent* pCharacterInfo);
		virtual ~EnemyGoToPlayerState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		void SetPlayer(CharacterInfoComponent* pPlayer);

	private:
		NodeComponent* GetNode(const glm::vec3& pos);
		void CalculatePath();
		bool IsOnLadder();
		
		size_t m_NextNodeIndex{};
		std::vector<NodeComponent*> m_pPath;
		CharacterInfoComponent* m_pPlayer{ nullptr };
	};
}