#include "Prefabs.h"
#include "Component/Physics/CharacterController2D.h"
#include "Components/PlayerController.h"
#include "Components/BurgerTimeMovementController.h"
#include "Components/BurgerPlate.h"

#include "Component/SpriteAtlasComponent.h"
#include "Component/RenderComponent.h"

dae::GameObject* dae::Prefabs::CreateBurgerPlate(Scene* pScene, RigidBody2DComponent* pRigidBody)
{
    GameObject* pObject{ pScene->Instantiate(pRigidBody->GetOwner()) };
    const glm::vec2 size{38.f, 38.f};
    BoxCollider2DComponent* pCollider{ pObject->AddComponent<BoxCollider2DComponent>() };
    pCollider->SetShape(pObject->AddComponent<QuadComponent>(size));
    pCollider->SetCollisionLayer(PLATE_COLLISION_LAYER);
    pCollider->SetCollisionIgnoreLayer(PLAYER_COLLISION_LAYER, true);
    pCollider->SetCollisionIgnoreLayer(LADDER_COLLISION_LAYER, true);
    pCollider->SetTrigger(true);
    pRigidBody->AddCollider(pCollider);
    pObject->AddComponent<BurgerPlate>(pRigidBody);
    return pObject;
}

dae::GameObject* dae::Prefabs::CreateBurgerIngredient(Scene* pScene, BurgerIngredient::IngredientType type)
{
    GameObject* pObject{ pScene->Instantiate() };
    RigidBody2DComponent* pRigidBody{ pObject->AddComponent<RigidBody2DComponent>() };
    TextureComponent* pTextureComponent{ pObject->AddComponent<TextureComponent>() };
    pTextureComponent->SetTexture("Textures/BurgerTimeCharacters.png");
    SpriteAtlasComponent* pSpriteAtlas{ pObject->AddComponent<SpriteAtlasComponent>(pTextureComponent)};

    pObject->AddComponent<BurgerIngredient>(type, pSpriteAtlas, pRigidBody);

    return pObject;
}

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

    BurgerTimeMovementController* pMovement{ pObject->AddComponent<BurgerTimeMovementController>(pCharacterController, PLAYER_COLLISION_LAYER) };
    pObject->AddComponent<PlayerController>(pMovement);

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
    pObject->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spritedID))->SetLayer(3);

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
