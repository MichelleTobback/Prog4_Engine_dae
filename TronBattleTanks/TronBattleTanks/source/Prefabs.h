#pragma once
#include "Scene/Scene.h"

#include <glm/glm.hpp>

namespace dae
{
	class TextComponent;
	class Prefab final
	{
	public:
		static GameObject* CreatePlayer(Scene* pScene, float health, int controllerIndex = -1);
		static GameObject* CreatePlayerHUD(Scene* pScene, GameObject* pPlayer1, GameObject* pPlayer2);
		static TextComponent* CreateTextObject(Scene* pScene, const std::string& txt = "", const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});
	};
}