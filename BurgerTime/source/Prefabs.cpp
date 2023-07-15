#include "Prefabs.h"
#include "Component/Physics/CharacterController2D.h"
#include "Components/PlayerController.h"

#include "Component/SpriteAtlasComponent.h"
#include "Component/RenderComponent.h"

dae::GameObject* dae::Prefabs::CreatePeterPepper(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };
    const float tileSize{ 16.f };

    CharacterController2D::CharacterController2DDesc controllerDesc{};
    controllerDesc.movementSpeed = 50.f;
    CharacterController2D* pCharacterController{ pObject->AddComponent<CharacterController2D>(controllerDesc) };
    pCharacterController->SetRigidBody(pObject->AddComponent<RigidBody2DComponent>());
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(glm::vec2{ tileSize, tileSize }));
    pCollider->SetCollisionLayer(PLAYER_COLLISION_LAYER);
    pCollider->SetTrigger(true);
    pCharacterController->SetCollider(pCollider);

    pObject->AddComponent<PlayerController>(pCharacterController);

    //sprites
    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTexture) };
    //const int rows{ 2 * tileSize }, cols{ 9 * tileSize };
    //for (int y{}; y < cols; ++y)
    //{
    //    for (int x{}; x < rows; ++x)
    //    {
    //
    //    }
    //}

    uint32_t spritedID{ pSpriteAtlas->AddSprite(0.f, 0.f, tileSize, tileSize) };
    pObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedID));

    return pObject;
}

dae::GameObject* dae::Prefabs::CreateTestLevel(Scene* pScene)
{
    GameObject* pObject{ pScene->Instantiate() };

    TextureComponent* pTexture{ pObject->AddComponent<TextureComponent>() };
    pTexture->SetTexture("Textures/BurgerTimeStages.png");
    SpriteComponent* pSprite{ pObject->AddComponent<SpriteComponent>(pTexture, 0.f, 0.f, 208.f, 200.f) };
    pObject->AddComponent<SpriteRenderComponent>(pSprite);

    return pObject;
}
