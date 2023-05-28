#pragma once

#include "Platform/ImGui/ImGuiComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Core/Delegate.h"

#include <unordered_map>

namespace dae
{
	using Ovec4 = ObservableType<glm::vec4(const glm::vec4)>;

	class QuadComponent;
	class TileGridEditor final : public ImGuiComponent
	{
	public:
		TileGridEditor(GameObject* pOwner);
		virtual ~TileGridEditor() {}

		TileGridEditor(const TileGridEditor& other) = delete;
		TileGridEditor(TileGridEditor&& other) = delete;
		TileGridEditor& operator=(const TileGridEditor& other) = delete;
		TileGridEditor& operator=(TileGridEditor&& other) = delete;

		virtual void Awake() override;

	protected:
		virtual void OnImGuiRender();

	private:
		int m_Rows{ 32 };
		int m_Cols{ 30 };
		float m_TileSize{ 16.f };
		//editorversion
		SpriteAtlasComponent* m_pSpriteAtlas{ nullptr };
		//scene version
		SpriteAtlasComponent* m_pSceneSpriteAtlas{ nullptr };
		GameObject* m_pGrid{ nullptr };

		glm::vec3 m_SpritesPos{};
		int m_pSelectedSprite{ 0 };
		QuadComponent* m_pTileQuad{ nullptr };

		bool m_MatchSpriteAtlas{true};

		std::unordered_map<int, SpriteComponent*> m_AddedSprites{}; //key = editorversion index, value = sceneversion ptr

		void CreateTileGrid();
		void CreateSpritesFromGrid();
		void AddSprite(const glm::vec4& source);
		void AddTile(const glm::vec4& source);

		SpriteComponent* AddSpriteFromAtlas(int index);

		SpriteComponent* FindMatchingSprite();
	};
}