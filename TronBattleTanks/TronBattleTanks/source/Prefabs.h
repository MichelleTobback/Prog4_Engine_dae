#pragma once
#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include <glm/glm.hpp>

namespace dae
{
	class SpriteAtlasComponent;
	class TextComponent;
	class Prefab final
	{
	public:
		static GameObject* CreateTestLevel(Scene* pScene);
		static GameObject* CreatePlayer(Scene* pScene, float health, int controllerIndex = -1);
		static GameObject* CreatePlayerHUD(Scene* pScene, GameObject* pPlayer1, GameObject* pPlayer2);
		static TextComponent* CreateTextObject(Scene* pScene, const std::string& txt = "", const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});
		static GameObject* CreateTextureRendererObject(Scene* pScene, const std::string& path);
		static GameObject* CreateSpriteObject(Scene* pScene, const std::string& path, const glm::vec4& src = {});
		static SpriteAtlasComponent* CreateSpriteAtlasObject(Scene* pScene, const std::string& path);

		static GameObject* CreateEnemyTank(Scene* pScene);
	};
}