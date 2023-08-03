#include "NodeComponent.h"
#include "Component/TransformComponent.h"
#include "Scene/GameObject.h"

#include <glm/gtx/norm.hpp>
#include <algorithm>

dae::Edge::Edge(NodeComponent* _pFrom, NodeComponent* _pTo)
	: pFrom{ _pFrom }, pTo{ _pTo }, cost{0}
{
	cost = glm::distance2(pFrom->GetTransform().GetWorldPosition(), pTo->GetTransform().GetWorldPosition());
}

dae::Edge::Edge(NodeComponent* _pFrom, NodeComponent* _pTo, float _cost)
	: pFrom{ _pFrom }, pTo{ _pTo }, cost{ _cost }
{
}

dae::NodeComponent::NodeComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

bool dae::NodeComponent::IsConnected(NodeComponent* pOther)
{
	return std::any_of(m_Edges.begin(), m_Edges.end(), [pOther](const Edge& edge)
		{
			return edge.pTo == pOther;
		});
}

dae::Edge* dae::NodeComponent::AddEdge(NodeComponent* pTo, bool directional)
{
	if (!IsConnected(pTo))
		m_Edges.push_back(Edge{ this, pTo });
	if (!directional)
		pTo->AddEdge(this);

	return &m_Edges.back();
}

void dae::NodeComponent::RemoveEdge(Edge* pEdge)
{
	m_Edges.erase(std::remove(m_Edges.begin(), m_Edges.end(), *pEdge), m_Edges.end());
}

std::vector<dae::Edge>& dae::NodeComponent::GetEdges()
{
	return m_Edges;
}

//===============================//
//		Serializable			 //
//===============================//

dae::NodeComponentSerializer::NodeComponentSerializer()
	: Serializable(Serializable::Create<NodeComponent>())
{
}

void dae::NodeComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
{
	NodeComponent* pNode{ pComponent->As<NodeComponent>() };
	const auto& edges{ pNode->GetEdges() };
	std::vector<uint64_t> connectedUuids;
	for (auto& edge : edges)
		connectedUuids.push_back(edge.pTo->GetUUID());
	out.WriteArray(&connectedUuids[0], connectedUuids.size());
}

dae::Component* dae::NodeComponentSerializer::Deserialize(DeserializeParams& params)
{
	if (params.pGameObject)
	{
		std::vector<uint64_t> connectedUuids;
		params.in.ReadArray(connectedUuids);

		NodeComponent* pInstance{ params.pGameObject->AddComponent<NodeComponent>() };

		for (uint64_t uuid : connectedUuids)
		{
			auto waitForConnection{ std::make_unique<OnGameObjectDeserialized>(UUID(uuid), params.pGameObject->GetScene(),
			std::vector<std::string>{Component::GetName<NodeComponent>()},
			[pInstance](GameObject* pObj)->bool
			{
				if (pObj->HasComponent<NodeComponent>())
				{
					pInstance->AddEdge(pObj->GetComponent<NodeComponent>());
					return true;
				}
				return false;
			}) };

			params.outOnDeserialized.push(std::move(waitForConnection));
		}
	}

	return nullptr;
}
