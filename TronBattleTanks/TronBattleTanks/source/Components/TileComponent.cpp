#include "TileComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "GridLevelComponent.h"
#include "Component/RenderComponent.h"
#include "Core/BitFlag.h"
#include <memory>

dae::TileComponent::TileComponent(GameObject* pOwner)
	: TileComponent(pOwner, TileDesc())
{

}

dae::TileComponent::TileComponent(GameObject* pOwner, const TileDesc& desc)
	: Component(pOwner)
	, m_Desc{ desc }
{
	
}

struct TileBlock
{
	uint64_t quadUuid{0u};
	uint64_t spriteUuid{ 0u };
	int flags{};
};

dae::TileDesc& dae::TileComponent::GetTileDesc()
{
	return m_Desc;
}

dae::TileComponentSerializer::TileComponentSerializer()
	: Serializable{ Serializable::Create<TileComponent>() }
{
}

void dae::TileComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
{
	TileComponent* pSpriteComponent{ pComponent->As<TileComponent>() };
	TileBlock block{};
	TileDesc desc{ pSpriteComponent->GetTileDesc() };
	block.flags = static_cast<int>(desc.flags);
	block.quadUuid = (desc.pQuadRenderer) ? desc.pQuadRenderer->GetUUID() : UUID(0u);
	block.spriteUuid = (desc.pRenderer) ? desc.pRenderer->GetUUID() : UUID(0u);
	out.Write(block);
}

dae::Component* dae::TileComponentSerializer::Deserialize(DeserializeParams& params)
{
	if (params.pGameObject)
	{
		TileBlock block{};
		params.in.Read(block);

		TileDesc desc{};
		desc.flags = static_cast<TileFlag>(block.flags);
		TileComponent* pInstance{ params.pGameObject->AddComponent<TileComponent>(desc) };

		auto waitForQuad{ std::make_unique<OnGameObjectDeserialized>(UUID(block.quadUuid), params.pGameObject->GetScene(),
			std::vector<std::string>{Component::GetName<QuadComponent>()},
			[pInstance](GameObject* pObj)->bool
			{
				if (pObj->HasComponent<QuadRendererComponent>())
				{
					pInstance->GetTileDesc().pQuadRenderer = pObj->GetComponent<QuadRendererComponent>();
					return true;
				}
				return false;
			}) };

		auto waitForSprite{ std::make_unique<OnGameObjectDeserialized>(block.spriteUuid, params.pGameObject->GetScene(),
			std::vector<std::string>{Component::GetName<SpriteComponent>()},
			[pInstance](GameObject* pObj)->bool
			{
				if (pObj->HasComponent<SpriteRenderComponent>())
				{
					pInstance->GetTileDesc().pRenderer = pObj->GetComponent<SpriteRenderComponent>();
					return true;
				}
				return false;
			}) };

		params.outOnDeserialized.push(std::move(waitForQuad));
		params.outOnDeserialized.push(std::move(waitForSprite));

		return pInstance;
	}
	return nullptr;
}
