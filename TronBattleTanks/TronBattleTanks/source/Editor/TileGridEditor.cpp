#include "TileGridEditor.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Component/RenderComponent.h"
#include "Managers/ServiceLocator.h"
#include "Component/UI/ButtonComponent.h"
#include "Components/TileComponent.h"
#include "Core/BitFlag.h"
#include "Input/Input.h"

dae::TileGridEditor::TileGridEditor(GameObject* pOwner)
	: ImGuiComponent(pOwner, "TileGrid editor")
{
	
}

void dae::TileGridEditor::Awake()
{
	m_pGrid = GetScene()->Instantiate();
	auto pTextureComponent{ m_pGrid->AddComponent<TextureComponent>() };
	pTextureComponent->SetTexture("Sprites/TronBattleTanksLevel.png");

	m_pTileQuad = m_pGrid->AddComponent<QuadComponent>(glm::vec2{ m_TileSize, m_TileSize });

	//editor version
	Window& window{ ServiceLocator::GetWindow() };
	GameObject* pAtlas{ GetScene()->Instantiate(GetOwner(), {window.GetWidth(), 0.f, 0.f})};
	pAtlas->AddComponent<TextureRenderComponent>(pTextureComponent);
	m_pSpriteAtlas = pAtlas->AddComponent<SpriteAtlasComponent>(pTextureComponent);
	//scene version
	m_pSceneSpriteAtlas = GetScene()->Instantiate(m_pGrid)->AddComponent<SpriteAtlasComponent>(pTextureComponent);

	//CreateSpritesFromGrid();
	//CreateTileGrid();
}

void dae::TileGridEditor::OnImGuiRender()
{
	ImGui::Text("Grid settings");
	ImGui::DragInt("cols", &m_Cols);
	ImGui::DragInt("rows", &m_Rows);
	ImGui::DragFloat("Tile size", &m_TileSize);
	ImGui::Checkbox("Match grid to atlas", &m_MatchSpriteAtlas);
	if (ImGui::Button("Create grid"))
	{
		CreateTileGrid();
	}

	ImGui::Separator();
	ImGui::Text("Sprite atlas settings");
	if (ImGui::Button("Sprites from grid"))
	{
		CreateSpritesFromGrid();
	}
}

void dae::TileGridEditor::CreateTileGrid()
{
	glm::vec3 pos{};

	for (int row{}; row < m_Rows; ++row)
	{
		for (int cols{}; cols < m_Cols; ++cols)
		{
			AddTile({ pos.x, pos.y, m_TileSize, m_TileSize });
			pos.x += m_TileSize;
		}
		pos.y += m_TileSize;
		pos.x = 0.f;
	}
}

void dae::TileGridEditor::CreateSpritesFromGrid()
{
	glm::vec4 source{0.f, 0.f, m_TileSize, m_TileSize };
	for (int row{}; row < m_Rows; ++row)
	{
		for (int cols{}; cols < m_Cols; ++cols)
		{
			AddSprite(source);
			source.x += m_TileSize;
		}
		source.y += m_TileSize;
		source.x = 0.f;
	}
}

void dae::TileGridEditor::AddSprite(const glm::vec4& source)
{
	uint32_t spriteIndex{ m_pSpriteAtlas->AddSprite(source) };
	glm::vec3 pos{ source.x, source.y, 0.f };

	auto pButtonObj{ GetScene()->Instantiate(m_pSpriteAtlas->GetOwner(), pos)};
	auto pButton{ pButtonObj->AddComponent<dae::ButtonComponent>(glm::vec2{source.z, source.w}) };
	dae::UIButtonLayout layout{};
	layout.color = { 0.01f, 0.01f, 0.01f, 1.f };
	//layout.pSprite = pSprite;
	pButton->SetLayout(layout);
	layout.color = { 0.5f, 0.5f, 0.f, 1.f };
	pButton->SetLayout(layout, dae::UIButtonFlag::Hovered);

	pButton->GetOnPressedDelegate() += [this, spriteIndex]() {m_pSelectedSprite = spriteIndex; };
}

void dae::TileGridEditor::AddTile(const glm::vec4& source)
{
	glm::vec3 pos{ source.x, source.y, 0.f };

	auto pTileObj{ GetScene()->Instantiate(m_pGrid, pos) };
	TileDesc desc{};
	desc.pRenderer = pTileObj->AddComponent<SpriteRenderComponent>(m_pSpriteAtlas->GetSprite(0));
	BitFlag::Set(desc.flags, TileFlag::Sprite, true);
	BitFlag::Set(desc.flags, TileFlag::Quad, false);
	TileComponent* pTile{ pTileObj->AddComponent<TileComponent>(desc) };

	auto pButtonObj{ GetScene()->Instantiate(GetOwner(), pos)};
	auto pButton{ pButtonObj->AddComponent<dae::ButtonComponent>(glm::vec2{source.z, source.w}) };
	dae::UIButtonLayout layout{};
	layout.color = { 1.f, 1.f, 1.f, 1.f };
	pButton->SetLayout(layout);
	layout.color = { 1.f, 0.f, 0.3f, 1.f };
	pButton->SetLayout(layout, dae::UIButtonFlag::Hovered);
	
	pButton->GetOnPressedDelegate() += [this, pTile, pButton]()
	{
		if (m_MatchSpriteAtlas)
		{
			glm::vec2 gridPos{ Input::GetInstance().GetMouse()->GetMousePos() / m_TileSize };
			int index{ static_cast<int>(gridPos.y) * m_Cols + static_cast<int>(gridPos.x) };
			pTile->GetTileDesc().pRenderer->SetSpriteComponent(AddSpriteFromAtlas(index));
		}
		else
			pTile->GetTileDesc().pRenderer->SetSpriteComponent(AddSpriteFromAtlas(m_pSelectedSprite));
	};
}

dae::SpriteComponent* dae::TileGridEditor::AddSpriteFromAtlas(int index)
{
	SpriteComponent* pReturnValue{ nullptr };
	if (m_AddedSprites.find(index) != m_AddedSprites.end())
	{
		pReturnValue = m_AddedSprites[index];
	}
	else
	{
		glm::vec4 source{ m_pSpriteAtlas->GetSprite(index)->GetSource() };
		uint32_t i{ m_pSceneSpriteAtlas->AddSprite(source.x, source.y, source.z, source.z) };
		pReturnValue = m_pSceneSpriteAtlas->GetSprite(i);
		m_AddedSprites.emplace(index, pReturnValue);
	}
	return pReturnValue;
}

dae::SpriteComponent* dae::TileGridEditor::FindMatchingSprite()
{
	return nullptr;
}
