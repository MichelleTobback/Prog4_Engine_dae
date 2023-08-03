#include "AStar.h"
#include "Component/TransformComponent.h"
#include <glm/gtx/norm.hpp>

std::vector<dae::NodeComponent*> dae::AStar::FindPath(NodeComponent* pStartNode, NodeComponent* pDestinationNode)
{
	std::vector<NodeComponent*> finalPath;
	std::vector<NodeRecord> openList;
	std::vector<NodeRecord> closedList;
	NodeRecord currentRecord;

	//setup startrecord
	NodeRecord startRecord;
	startRecord.pNode = pStartNode;
	startRecord.pConnection = nullptr;
	startRecord.estimatedTotalCost = glm::distance2(pStartNode->GetTransform().GetWorldPosition(), pDestinationNode->GetTransform().GetWorldPosition());

	openList.push_back(startRecord);

	while (!openList.empty())
	{
		currentRecord = *std::min_element(openList.begin(), openList.end());

		if (currentRecord.pNode == pDestinationNode)
			break;

		else
		{
			auto& allConnections{ currentRecord.pNode->GetEdges() };
			for (auto& connection : allConnections)
			{
				bool alreadyInClosed{ false };
				bool existingIsCheaper{ false };
				auto connectedNode{ connection.pTo };
				float connectedCostSoFar = connection.cost + currentRecord.costSoFar;

				for (const NodeRecord& nodeRecordInClosed : closedList)
				{
					if (nodeRecordInClosed.pNode == connectedNode)
					{
						alreadyInClosed = true;
						if (nodeRecordInClosed.costSoFar > connectedCostSoFar)
						{
							closedList.erase(std::remove(closedList.begin(), closedList.end(), nodeRecordInClosed));
						}
						else
						{
							existingIsCheaper = true;
						}
						break;
					}
				}
				if (!existingIsCheaper && !alreadyInClosed)
				{
					for (const NodeRecord& nodeRecordInOpen : openList)
					{
						if (nodeRecordInOpen.pNode == connectedNode)
						{
							if (nodeRecordInOpen.costSoFar > connectedCostSoFar)
							{
								openList.erase(std::remove(openList.begin(), openList.end(), nodeRecordInOpen));
							}
							else
							{
								existingIsCheaper = true;
							}
							break;
						}
					}
				}

				if (existingIsCheaper)
					continue;

				NodeRecord connectionRecord;
				connectionRecord.pNode = connectedNode;
				connectionRecord.pConnection = &connection;
				connectionRecord.costSoFar = connectedCostSoFar;
				openList.push_back(connectionRecord);
			}
			for (const NodeRecord& nodeRecordInOpen : openList)
			{
				if (nodeRecordInOpen.pNode == currentRecord.pNode)
				{
					currentRecord = nodeRecordInOpen;
					//add noderecord to closedList
					closedList.push_back(currentRecord);
					//remove record from openList
					openList.erase(std::remove(openList.begin(), openList.end(), nodeRecordInOpen));
					break;
				}
			}
		}
	}

	// backtracking
	while (currentRecord.pNode != pStartNode)
	{
		//assert(finalPath.size() == 0 || (finalPath.size() > 0 && currentRecord.pNode != finalPath.back() && "Same node!"));
		finalPath.push_back(currentRecord.pNode);

		for (const NodeRecord& nodeRecordInClosed : closedList)
		{
			if (nodeRecordInClosed.pNode == currentRecord.pConnection->pFrom)
			{
				currentRecord = nodeRecordInClosed;
				closedList.erase(std::remove(closedList.begin(), closedList.end(), nodeRecordInClosed));
				break;
			}
		}
	}
	finalPath.push_back(pStartNode);
	std::reverse(finalPath.begin(), finalPath.end());

	return finalPath;
}
