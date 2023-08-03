#include "HUDComponent.h"
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

#include "Component/RenderComponent.h"
#include "Component/SpriteAtlasComponent.h"

dae::HUDComponent::HUDComponent(GameObject* pOwner, CharacterInfo* pPlayer)
	: Component(pOwner)
{
}

void dae::HUDComponent::Awake()
{


    GameObject* pObject{ GetOwner()->GetScene()->Instantiate(0u, GetOwner())};

    const float tileSize{ 8.f };
    const float spriteSize{ tileSize * 2.f };
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };

    const glm::vec3 lifePos{};
    for (int i{}; i < )
    {
        uint32_t spritedLifeID{ pSpriteAtlas->AddSprite(0.f, 0.f, tileSize, tileSize) };
        GameObject* pSpriteObject{ pObject->GetScene()->Instantiate(pObject, glm::vec3{0.f, -tileSize, 0.f}) };
        SpriteRenderComponent* pRenderer{ pSpriteObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedLifeID)) };
    }
}
