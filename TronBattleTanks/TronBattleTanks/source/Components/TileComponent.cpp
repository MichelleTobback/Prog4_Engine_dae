#include "TileComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "GridLevelComponent.h"
#include "Component/RenderComponent.h"
#include "Core/BitFlag.h"

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

void dae::TileComponent::Serialize(BinaryWriter& out) const
{
	TileBlock block{};
	block.flags = static_cast<int>(m_Desc.flags);
	block.quadUuid = (m_Desc.pQuadRenderer) ? m_Desc.pQuadRenderer->GetUUID() : UUID(0u);
	block.spriteUuid = (m_Desc.pRenderer) ? m_Desc.pRenderer->GetUUID() : UUID(0u);
	out.Write(block);
}

bool dae::TileComponent::Deserialize(DeserializeParams& params)
{
	if (params.componentName._Equal(typeid(TileComponent).name()))
	{
		TileBlock block{};
		params.in.Read(block);

		TileDesc desc{};
		desc.flags = static_cast<TileFlag>(block.flags);
		auto pInstance{ params.pGameObject->AddComponent<TileComponent>(desc) };

		OnGameObjectDeserialized waitForQuad{ block.quadUuid, params.pGameObject->GetScene(), [pInstance](GameObject* pObj, Scene*)->bool
			{
				if (pObj->HasComponent<QuadRendererComponent>())
				{
					pInstance->GetTileDesc().pQuadRenderer = pObj->GetComponent<QuadRendererComponent>();
					return true;
				}
				return false;
			}};

		OnGameObjectDeserialized waitForSprite{ block.spriteUuid, params.pGameObject->GetScene(), [pInstance](GameObject* pObj, Scene*)->bool
			{
				if (pObj->HasComponent<SpriteRenderComponent>())
				{
					pInstance->GetTileDesc().pRenderer = pObj->GetComponent<SpriteRenderComponent>();
					return true;
				}
				return false;
			} };

		params.outOnDeserialized.push_back(waitForQuad);
		params.outOnDeserialized.push_back(waitForSprite);

		return true;
	}
	return false;
}

dae::TileDesc& dae::TileComponent::GetTileDesc()
{
	return m_Desc;
}
