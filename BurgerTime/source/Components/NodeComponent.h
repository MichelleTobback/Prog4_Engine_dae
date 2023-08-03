#pragma once
#include "Component/Component.h"
#include "Scene/Serializable.h"

namespace dae
{
	class NodeComponent;
	struct Edge
	{
		Edge() = default;
		Edge(NodeComponent* _pFrom, NodeComponent* _pTo);
		Edge(NodeComponent* _pFrom, NodeComponent* _pTo, float _cost);

		bool operator==(const Edge& other) const
		{
			return pFrom == other.pFrom && pTo == other.pTo;
		}

		NodeComponent* pFrom{ nullptr };
		NodeComponent* pTo{ nullptr };
		float cost{};
	};

	class NodeComponent final : public Component
	{
	public:
		NodeComponent(GameObject* pOwner);
		virtual ~NodeComponent() = default;

		NodeComponent(const NodeComponent& other) = delete;
		NodeComponent(NodeComponent&& other) = delete;
		NodeComponent& operator=(const NodeComponent& other) = delete;
		NodeComponent& operator=(NodeComponent&& other) = delete;

		bool IsConnected(NodeComponent* pOther);
		Edge* AddEdge(NodeComponent* pTo, bool directional = true);
		void RemoveEdge(Edge* pEdge);
		std::vector<Edge>& GetEdges();

	private:
		std::vector<Edge> m_Edges;
	};

	//===============================//
	//		Serializable			 //
	//===============================//

	class NodeComponentSerializer final : public Serializable
	{
	public:
		NodeComponentSerializer();
		virtual ~NodeComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;
	};
}