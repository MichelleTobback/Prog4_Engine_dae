#include "TileComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "GridLevelComponent.h"
#include "Component/RenderComponent.h"

dae::TileComponent::TileComponent(GameObject* pOwner, GridLevelComponent* pGrid)
	: TileComponent(pOwner, pGrid, TileDesc())
{

}

dae::TileComponent::TileComponent(GameObject* pOwner, GridLevelComponent* pGrid, const TileDesc& desc)
	: Component(pOwner)
	, m_Desc{ desc }
	, m_pGrid{ pGrid }
{
	auto& gridDesc{ pGrid->GetDesc() };
	auto pSprite{ gridDesc.pSpriteAtlas->GetSprite(desc.m_SpriteIndex) };
	m_pRenderer = GetOwner()->AddComponent<SpriteRenderComponent>(pSprite);
}

void dae::TileComponent::SetWidth(float width)
{
	m_Desc.Width = width;
}

void dae::TileComponent::SetHeight(float height)
{
	m_Desc.Height = height;
}

void dae::TileComponent::SetSize(float width, float height)
{
	m_Desc.Width = width;
	m_Desc.Height = height;
}
