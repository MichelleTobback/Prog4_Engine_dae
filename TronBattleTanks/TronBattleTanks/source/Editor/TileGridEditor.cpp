#include "TileGridEditor.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Component/RenderComponent.h"
#include "Managers/ServiceLocator.h"
#include "Component/UI/ButtonComponent.h"
#include "Components/TileComponent.h"
#include "Component/QuadComponent.h"
#include "Core/BitFlag.h"
#include "Input/Input.h"
#include "Component/Physics/RigidBody2DComponent.h"

dae::TileGridEditor::TileGridEditor(GameObject* pOwner)
	: ImGuiComponent(pOwner, "TileGrid editor")
{
	m_pOnGameObjectSelected = std::make_unique<Delegate<void(GameObject*)>>();
}

void dae::TileGridEditor::Awake()
{
	m_pGrid = GetScene()->Instantiate();
	auto pTextureComponent{ m_pGrid->AddComponent<TextureComponent>() };
	pTextureComponent->SetTexture("Sprites/TronBattleTanksLevel.png");
	m_pGridRigidBody = m_pGrid->AddComponent<RigidBody2DComponent>();
	m_pGridRigidBody->SetStatic();

	m_pTileQuad = GetScene()->Instantiate(GetOwner())->AddComponent<QuadComponent>(glm::vec2{m_TileSize, m_TileSize});
	m_pTileQuad->SetColor({ 0.6f, 0.5f, 0.2f, 1.f });

	//editor version
	Window& window{ ServiceLocator::GetWindow() };
	GameObject* pAtlas{ GetScene()->Instantiate(GetOwner(), {window.GetWidth(), 0.f, 0.f})};
	pAtlas->AddComponent<TextureRenderComponent>(pTextureComponent);
	m_pSpriteAtlas = pAtlas->AddComponent<SpriteAtlasComponent>(pTextureComponent);
	//scene version
	m_pSceneSpriteAtlas = GetScene()->Instantiate(m_pGrid)->AddComponent<SpriteAtlasComponent>(pTextureComponent);

	CreateSpritesFromGrid();
	CreateTileGrid();

	m_pTileQuadRenderer = GetScene()->Instantiate(GetOwner())->AddComponent<QuadRendererComponent>(m_pTileQuad);
	m_pSpriteQuadRenderer = GetScene()->Instantiate(GetOwner(), m_pGrid->GetTransform().GetWorldPosition())->AddComponent<QuadRendererComponent>(m_pTileQuad);
}

void dae::TileGridEditor::OnImGuiRender()
{
	ImGui::Text("Grid settings");
	ImGui::Checkbox("Match grid to atlas", &m_MatchSpriteAtlas);

	if (ImGui::BeginCombo("Mode", GetEditorModeName(m_Mode).c_str()))
	{
		for (int mode{ 0 }; mode <= static_cast<int>(EditorMode::Last); ++mode)
		{
			EditorMode currentMode{ static_cast<EditorMode>(mode) };
			bool selected{ m_Mode == currentMode };
			if (ImGui::Selectable(GetEditorModeName(currentMode).c_str(), &selected))
			{
				m_Mode = currentMode;
			}

		}
		ImGui::EndCombo();
	}

	glm::vec2 size{ m_pTileQuad->GetSize() / m_TileSize };
	if (ImGui::DragFloat2("TileSize", &size.x, 1.f, 1.f, m_Cols * 1.f))
	{
		m_pTileQuad->SetSize(size * m_TileSize);
	}

	switch (m_Mode)
	{
	case EditorMode::Paint:
	{
		ImGui::Checkbox("Solid", &m_Solid);
	}
	break;

	case EditorMode::Rotate:
	{
		ImGui::DragFloat("current rotation", &m_CurrentRotation, 90.f, 0.f, 270.f);
	}
	break;
	}

}

void dae::TileGridEditor::CreateTileGrid()
{
	glm::vec3 pos{};
	m_pGridObjects.resize(m_Rows * m_Cols, {});

	for (int row{}; row < m_Rows; ++row)
	{
		for (int cols{}; cols < m_Cols; ++cols)
		{
			int index{ row * m_Cols + cols };
			AddTile({ pos.x, pos.y, m_TileSize, m_TileSize }, index);
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
	glm::vec3 pos{ source.x, source.y, 0.f };

	auto pButtonObj{ GetScene()->Instantiate(m_pSpriteAtlas->GetOwner(), pos)};
	auto pButton{ pButtonObj->AddComponent<dae::ButtonComponent>(glm::vec2{source.z, source.w}) };
	dae::UIButtonLayout layout{};
	layout.color = { 0.01f, 0.01f, 0.01f, 1.f };
	//layout.pSprite = pSprite;
	pButton->SetLayout(layout);
	layout.color = { 0.5f, 0.5f, 0.f, 1.f };
	pButton->SetLayout(layout, dae::UIButtonFlag::Hovered);

	pButton->GetOnPressedDelegate() += [this, pButton]()
	{
		if (!m_MatchSpriteAtlas)
			m_pSpriteQuadRenderer->GetOwner()->GetTransform().SetLocalPosition(pButton->GetOwner()->GetTransform().GetWorldPosition());
	};
}

void dae::TileGridEditor::AddTile(const glm::vec4& source, int index)
{
	glm::vec3 pos{ source.x, source.y, 0.f };

	auto pButtonObj{ GetScene()->Instantiate(GetOwner(), pos)};
	auto pButton{ pButtonObj->AddComponent<dae::ButtonComponent>(glm::vec2{source.z, source.w}) };
	dae::UIButtonLayout layout{};
	layout.color = { 1.f, 1.f, 1.f, 1.f };
	pButton->SetLayout(layout);
	layout.color = { 1.f, 0.f, 0.3f, 1.f };
	pButton->SetLayout(layout, dae::UIButtonFlag::Hovered);
	
	pButton->GetOnPressedDelegate() += [this, index, pButton]()->void
	{
		//get object
		if (m_pGridObjects[index].pObject == nullptr)
		{
			m_pGridObjects[index].pObject = GetScene()->Instantiate(m_pGrid, pButton->GetOwner()->GetTransform().GetWorldPosition());
		}
		TileData& tile{ m_pGridObjects[index] };
		m_pSelectedTile = tile.pObject;
		m_pOnGameObjectSelected->Invoke(m_pSelectedTile);

		switch (m_Mode)
		{
		case EditorMode::Paint:
		{
			//Sprite renderer
			if (tile.pRenderer == nullptr)
			{
				tile.pRenderer = tile.pObject->AddComponent<SpriteRenderComponent>();
			}
			tile.pRenderer->SetSpriteComponent(AddOrCreateSprite());

			if (m_Solid && tile.pCollider == nullptr)
			{
				tile.pCollider = tile.pObject->AddComponent<BoxCollider2DComponent>();
				auto pQuad{ tile.pObject->AddComponent<QuadComponent>(m_pTileQuad->GetSize()) };
				tile.pCollider->SetShape(pQuad);
				m_pGridRigidBody->AddCollider(tile.pCollider);
			}
		}
			break;

		case EditorMode::Rotate:
		{
			tile.pObject->GetTransform().SetLocalRotation(m_CurrentRotation);
		}
			break;
		}
	};

	pButton->GetOnHoveredDelegate() += [this, pButton]()
	{
		m_pTileQuadRenderer->GetOwner()->GetTransform().SetLocalPosition(pButton->GetOwner()->GetTransform().GetWorldPosition());

		if (m_MatchSpriteAtlas)
		{
			m_pSpriteQuadRenderer->GetOwner()->GetTransform().SetLocalPosition(
				m_pTileQuadRenderer->GetOwner()->GetTransform().GetWorldPosition() + m_pSpriteAtlas->GetOwner()->GetTransform().GetWorldPosition());
		}
	};
}

dae::SpriteComponent* dae::TileGridEditor::AddOrCreateSprite()
{
	SpriteComponent* pReturnValue{ nullptr };
	glm::vec3 worldPos{ m_pSpriteQuadRenderer->GetOwner()->GetTransform().GetWorldPosition() - m_pSpriteAtlas->GetOwner()->GetTransform().GetWorldPosition() };
	worldPos /= m_TileSize;
	glm::ivec4 dimensions{};
	dimensions.x = static_cast<int>(worldPos.x);
	dimensions.y = static_cast<int>(worldPos.y);
	dimensions.z = static_cast<int>(m_pTileQuad->GetSize().x / m_TileSize);
	dimensions.w = static_cast<int>(m_pTileQuad->GetSize().y / m_TileSize);

	for (auto& [quad, pSprite] : m_AddedSprites)
	{
		if (dimensions.x == quad.x && dimensions.y == quad.y
			&& dimensions.z == quad.z && dimensions.w == quad.w)
		{
			return pSprite;
		}
	}

	glm::vec4 source{};
	source.x = dimensions.x * m_TileSize;
	source.y = dimensions.y * m_TileSize;
	source.z = dimensions.z * m_TileSize;
	source.w = dimensions.w * m_TileSize;
	uint32_t i{ m_pSceneSpriteAtlas->AddSprite(source.x, source.y, source.z, source.w) };
	pReturnValue = m_pSceneSpriteAtlas->GetSprite(i);
	m_AddedSprites.push_back(std::make_pair(dimensions, pReturnValue));

	return pReturnValue;
}

std::string dae::TileGridEditor::GetEditorModeName(EditorMode mode) const
{
	switch (static_cast<EditorMode>(mode))
	{
	case EditorMode::Select:
		return "Select";
		break;

	case EditorMode::Paint:
		return "Paint";
		break;

	case EditorMode::Rotate:
		return "Rotate";
		break;
	}
	return "";
}
