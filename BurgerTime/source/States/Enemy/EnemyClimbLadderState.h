#pragma once
#include "States/CharacterState.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class NodeComponent;
	class EnemyClimbLadderState final : public CharacterState
	{
	public:
		EnemyClimbLadderState(CharacterInfoComponent* pCharacterInfo);
		virtual ~EnemyClimbLadderState() = default;

		virtual void OnEnter() override;
		virtual StatePtr OnUpdate() override;
		virtual void OnExit() override;

		void SetEndTile(NodeComponent* pNode);

	private:
		NodeComponent* GetNode(const glm::vec3& pos);

		size_t m_NextNodeIndex{};
		glm::vec3 m_EndPos{};
		glm::vec2 m_MoveDir{};
	};
}