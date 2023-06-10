#pragma once

#include "Platform/ImGui/ImGuiComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Core/Delegate.h"

#include <unordered_map>

namespace dae
{
	using Ovec4 = ObservableType<glm::vec4(const glm::vec4)>;

	enum class EditorMode
	{
		Select = 0,
		Paint = 1,
		Rotate = 2,

		Last = Rotate
	};

	class RigidBody2DComponent;
	class BoxCollider2DComponent;
	class QuadRendererComponent;
	class SpriteRenderComponent;
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

		Delegate<void(GameObject*)>& GetOnGameObjectSelected() { return *m_pOnGameObjectSelected; }

	protected:
		virtual void OnImGuiRender();

	private:
		struct TileData
		{
			GameObject* pObject{ nullptr };
			SpriteRenderComponent* pRenderer{ nullptr };
			BoxCollider2DComponent* pCollider{ nullptr };
		};

		int m_Rows{ 32 };
		int m_Cols{ 30 };
		float m_TileSize{ 16.f };
		//editorversion
		SpriteAtlasComponent* m_pSpriteAtlas{ nullptr };
		//scene version
		SpriteAtlasComponent* m_pSceneSpriteAtlas{ nullptr };
		GameObject* m_pGrid{ nullptr };
		RigidBody2DComponent* m_pGridRigidBody{ nullptr };

		glm::vec3 m_SpritesPos{};
		GameObject* m_pSelectedTile{ 0 };
		QuadComponent* m_pTileQuad{ nullptr };
		QuadRendererComponent* m_pTileQuadRenderer{ nullptr };
		QuadRendererComponent* m_pSpriteQuadRenderer{ nullptr };

		bool m_MatchSpriteAtlas{true};
		
		EditorMode m_Mode{ 0 };
		float m_CurrentRotation{ 0.f };
		bool m_Solid{ true };

		//std::unordered_map<int, SpriteComponent*> m_AddedSprites{}; //key = editorversion index, value = sceneversion ptr
		std::vector<std::pair<glm::ivec4, SpriteComponent*>> m_AddedSprites{}; // in grid dimensions
		std::vector<TileData> m_pGridObjects{};

		std::unique_ptr<Delegate<void(GameObject*)>> m_pOnGameObjectSelected{ nullptr };

		void CreateTileGrid();
		void CreateSpritesFromGrid();
		void AddSprite(const glm::vec4& source);
		void AddTile(const glm::vec4& source, int index);

		SpriteComponent* AddOrCreateSprite();

		std::string GetEditorModeName(EditorMode mode) const;
	};
}