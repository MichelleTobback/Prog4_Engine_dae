#pragma once

#include "Component.h"
#include "TextureComponent.h"
#include "SpriteComponent.h"

#include <glm/glm.hpp>
#include <vector>

namespace dae
{
	class SpriteAtlasComponent final : public Component
	{
	public:
		SpriteAtlasComponent(GameObject* pOwner, TextureComponent* pTexture);
		virtual ~SpriteAtlasComponent() override = default;

		SpriteAtlasComponent(const SpriteAtlasComponent& other) = delete;
		SpriteAtlasComponent(SpriteAtlasComponent&& other) = delete;
		SpriteAtlasComponent& operator=(const SpriteAtlasComponent& other) = delete;
		SpriteAtlasComponent& operator=(SpriteAtlasComponent&& other) = delete;

		void SetTexture(TextureComponent* pTexture);
		//return index
		uint32_t AddSprite(const glm::vec4& src);
		//return index
		uint32_t AddSprite(float srcX = 0.f, float srcY = 0.f, float srcWidth = 0.f, float srcHeight = 0.f);
		uint32_t AddSprite(SpriteComponent* pSprite);

		inline SpriteComponent* GetSprite(uint32_t id) const { return m_pSprites[id]; }
		inline const TextureComponent& GetTexture() const { return *m_pTexture; }
		inline const std::vector<SpriteComponent*>& GetSprites() const { return m_pSprites; }

	private:
		TextureComponent* m_pTexture{ nullptr };
		std::vector<SpriteComponent*> m_pSprites{};
	};
}