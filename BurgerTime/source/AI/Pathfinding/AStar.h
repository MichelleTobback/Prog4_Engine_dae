#pragma once
#include "Components/NodeComponent.h"

namespace dae
{
	class AStar final
	{
	public:
		AStar() = default;

		struct NodeRecord
		{
			NodeComponent* pNode{nullptr};
			Edge* pConnection{ nullptr };
			float costSoFar{}; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost{}; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<NodeComponent*> FindPath(NodeComponent* pStartNode, NodeComponent* pDestinationNode);

	private:

	};
}