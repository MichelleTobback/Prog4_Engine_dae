#include "SerializableComponents.h"
#include "Scene/GameObject.h"
#include "QuadComponent.h"
#include "RenderComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "SpriteAtlasComponent.h"
#include "Component/Physics/RigidBody2DComponent.h"
#include "Managers/ResourceManager.h"
#include "Core/BitFlag.h"

namespace dae
{
#pragma region Quad
    //=================================
    // Quad
    //=================================
    struct QuadBlock
    {
        glm::vec4 color{};
        glm::vec2 size{};
        bool isSolid{};
    };

    QuadSerializer::QuadSerializer()
        : Serializable{ Serializable::Create<QuadComponent>() }
    {
    }

    void QuadSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        auto pQuad{ pComponent->As<QuadComponent>() };
        QuadBlock block{};
        block.color = pQuad->GetColor();
        block.size = pQuad->GetSize();
        block.isSolid = pQuad->IsSolid();
        out.Write(block);
    }

    Component* QuadSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            QuadBlock block{};
            params.in.Read(block);

            return params.pGameObject->AddComponent<QuadComponent>(block.size, block.color, block.isSolid);
        }
        return nullptr;
    }
#pragma endregion //Quad

#pragma region TextureRenderer
    //=================================
    // TextureRenderer
    //=================================

    TextureRendererSerializer::TextureRendererSerializer()
        : Serializable{ Serializable::Create<TextureRenderComponent>() }
    {
    }

    void TextureRendererSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        TextureRenderComponent* pTextureRendererComponent{ pComponent->As<TextureRenderComponent>() };
        uint64_t textureUuid{ (pTextureRendererComponent->GetTexture()) ? pTextureRendererComponent->GetTexture()->GetUUID() : UUID(0u) };

        out.Write(textureUuid);
    }

    Component* TextureRendererSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            uint32_t quadUuid{};
            params.in.Read(quadUuid);

            TextureRenderComponent* pInstance{ params.pGameObject->AddComponent<TextureRenderComponent>() };

            auto waitForTexture{ std::make_unique<OnGameObjectDeserialized>(quadUuid, params.pGameObject->GetScene(),
            std::vector<std::string>{Component::GetName<TextureComponent>()},
            [pInstance, params](GameObject* pObj)->bool
            {
                if (!pObj->HasComponent<TextureComponent>())
                        return false;

                    TextureComponent* textureComponent{ pObj->GetComponent<TextureComponent>() };
                    pInstance->SetTexture(textureComponent);

                    return true;
            }) };

            params.outOnDeserialized.push(std::move(waitForTexture));

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //TextureRenderer

#pragma region SpriteRenderer
    //=================================
    // SpriteRenderer
    //=================================

    SpriteRendererSerializer::SpriteRendererSerializer()
        : Serializable{ Serializable::Create<SpriteRenderComponent>() }
    {
    }

    void SpriteRendererSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        SpriteRenderComponent* pSpriteRenderer{ pComponent->As<SpriteRenderComponent>() };
        uint64_t spriteUuid{ (pSpriteRenderer->GetSprite()) ? pSpriteRenderer->GetSprite()->GetUUID() : UUID(0u) };

        out.Write(spriteUuid);
    }

    Component* SpriteRendererSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            uint64_t spriteUuid{};
            params.in.Read(spriteUuid);

            SpriteRenderComponent* pInstance{ params.pGameObject->AddComponent<SpriteRenderComponent>() };

            auto waitForSprite{ std::make_unique<OnGameObjectDeserialized>(spriteUuid, params.pGameObject->GetScene(),
            std::vector<std::string>{Component::GetName<SpriteComponent>()},
            [pInstance, params](GameObject* pObj)->bool
            {
                if (!pObj->HasComponent<SpriteComponent>())
                    return false;

                SpriteComponent* pSpriteComponent{ pObj->GetComponent<SpriteComponent>() };
                pInstance->SetSpriteComponent(pSpriteComponent);

                return true;
            }) };
            params.outOnDeserialized.push(std::move(waitForSprite));

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //SpriteRenderer

#pragma region QuadRenderer
    //=================================
    // QuadRenderer
    //=================================

    QuadRendererSerializer::QuadRendererSerializer()
        : Serializable{ Serializable::Create<QuadRendererComponent>() }
    {
    }

    void QuadRendererSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        QuadRendererComponent* pQuadRendererComponent{ pComponent->As<QuadRendererComponent>() };
        uint64_t quadUuid{ (pQuadRendererComponent) ? static_cast<uint64_t>(pQuadRendererComponent->GetUUID()) : 0u };
        out.Write(quadUuid);
    }

    Component* QuadRendererSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            uint32_t quadUuid{};
            params.in.Read(quadUuid);

            QuadRendererComponent* pInstance{ params.pGameObject->AddComponent<QuadRendererComponent>() };

            auto waitForQuad{ std::make_unique<OnGameObjectDeserialized>(quadUuid, params.pGameObject->GetScene(),
                std::vector<std::string>{Component::GetName<QuadComponent>()},
                [pInstance, params](GameObject* pObj)->bool
                {
                    if (pObj->HasComponent<QuadComponent>())
                    {
                        pInstance->SetQuad(pObj->GetComponent<QuadComponent>());
                        return true;
                    }
                    return false;
                }) };

            params.outOnDeserialized.push(std::move(waitForQuad));

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //QuadRenderer

#pragma region Texture
    //=================================
    // Texture
    //=================================
    TextureSerializer::TextureSerializer()
        : Serializable{ Serializable::Create<TextureComponent>() }
    {
    }

    void TextureSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        TextureComponent* pTextureComponent{ pComponent->As<TextureComponent>() };
        bool validTexture{ pTextureComponent->HasTexture() };
        if (validTexture)
        {
            out.WriteString(pTextureComponent->GetTexture()->GetPath());
        }
        else
        {
            out.Write(0);
        }
    }

    Component* TextureSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            TextureComponent* pInstance{ params.pGameObject->AddComponent<TextureComponent>() };

            std::string texPath{};
            if (!params.in.ReadString(texPath).empty())
            {
                pInstance->SetTexture(texPath);
            }
            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //Texture

#pragma region TextComponent
    //=================================
    // TextComponent
    //=================================
    struct TextBlock
    {
        glm::vec4 color{};
        int fontSize{};
    };

    TextComponentSerializer::TextComponentSerializer()
        : Serializable{ Serializable::Create<TextComponent>() }
    {
    }

    void TextComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        TextComponent* pTextComponent{ pComponent->As<TextComponent>() };
        TextBlock txtData{};
        auto pFont{ pTextComponent->GetFont() };
        txtData.fontSize = pFont->GetSize();
        txtData.color = pTextComponent->GetColor();

        out.Write(txtData);
        out.WriteString(pFont->GetPath());
        out.WriteString(pTextComponent->GetText());
    }

    Component* TextComponentSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            TextBlock tcData{};
            params.in.Read(tcData);

            std::string fontPath{};
            params.in.ReadString(fontPath);
            std::string text{};
            params.in.ReadString(text);

            auto pFont{ ResourceManager::GetInstance().LoadFont(fontPath, static_cast<unsigned int>(tcData.fontSize)) };
           return  params.pGameObject->AddComponent<TextComponent>(text, std::move(pFont), tcData.color);
        }
        return nullptr;
    }
#pragma endregion //TextComponent

#pragma region SpriteComponent
    //=================================
    // SpriteComponent
    //=================================
    struct SpriteBlock
    {
        glm::vec4 source{};
        uint64_t textureUuid{};
    };

    SpriteComponentSerializer::SpriteComponentSerializer()
        : Serializable{ Serializable::Create<SpriteComponent>() }
    {
    }

    void SpriteComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        SpriteComponent* pSpriteComponent{ pComponent->As<SpriteComponent>() };

        SpriteBlock spriteData{};
        spriteData.source = pSpriteComponent->GetSource();
        auto pTexture{ pSpriteComponent->GetTexture() };
        spriteData.textureUuid = (pTexture) ? pTexture->GetUUID() : UUID(0u);

        out.Write(spriteData);
    }

    Component* SpriteComponentSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            SpriteBlock spriteData{};
            params.in.Read(spriteData);

            SpriteComponent* pInstance{ params.pGameObject->AddComponent<SpriteComponent>(nullptr, spriteData.source) };

            auto waitForTexture{ std::make_unique<OnGameObjectDeserialized>(spriteData.textureUuid, params.pGameObject->GetScene(),
                std::vector<std::string>{Component::GetName<TextureComponent>()},
                [pInstance](GameObject* pObj)->bool
                {
                    if (!pObj->HasComponent<TextureComponent>())
                        return false;

                    TextureComponent* textureComponent{ pObj->GetComponent<TextureComponent>() };
                    pInstance->SetTexture(textureComponent, false);

                    return true;
                }) };

            params.outOnDeserialized.push(std::move(waitForTexture));

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //SpriteComponent

#pragma region SpriteAtlasComponent
    //=================================
    // SpriteCSpriteAtlasComponentomponent
    //=================================
    struct SpriteAtlasBlock
    {
        uint64_t textureUuid{};
        int numSprites{};
    };

    SpriteAtlasComponentSerializer::SpriteAtlasComponentSerializer()
        : Serializable{ Serializable::Create<SpriteAtlasComponent>() }
    {
    }

    void SpriteAtlasComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        SpriteAtlasComponent* pSpriteAtlas{ pComponent->As<SpriteAtlasComponent>() };

        SpriteAtlasBlock data{};
        auto& pSprites{ pSpriteAtlas->GetSprites() };
        data.numSprites = static_cast<int>(pSprites.size());
        auto pTexture{ &pSpriteAtlas->GetTexture() };
        data.textureUuid = (pTexture) ? pTexture->GetUUID() : UUID(0u);
        std::vector<uint64_t> pSpriteUuids(data.numSprites);
        for (int i{}; i < data.numSprites; ++i)
        {
            pSpriteUuids[i] = pSprites[i]->GetUUID();
        }

        out.Write(data);

        if (pSpriteUuids.size() > 0)
            out.WriteArray(&pSpriteUuids[0], pSpriteUuids.size());
        else
            out.Write(0);
    }

    Component* SpriteAtlasComponentSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            SpriteAtlasBlock spriteAtlasData{};
            params.in.Read(spriteAtlasData);

            SpriteAtlasComponent* pInstance{ params.pGameObject->AddComponent<SpriteAtlasComponent>() };

            auto waitForTexture{ std::make_unique<OnGameObjectDeserialized>(spriteAtlasData.textureUuid, params.pGameObject->GetScene(),
                std::vector<std::string>{Component::GetName<TextureComponent>()},
                [pInstance, spriteAtlasData](GameObject* pObj)->bool
                {
                    if (!pObj->HasComponent<TextureComponent>())
                        return false;

                    TextureComponent* pTextureComponent{ pObj->GetComponent<TextureComponent>() };
                    pInstance->SetTexture(pTextureComponent);

                    return true;
                }) };

            params.outOnDeserialized.push(std::move(waitForTexture));

            std::vector<uint64_t> spritesArr(spriteAtlasData.numSprites);
            params.in.ReadArray(spritesArr);

            std::for_each(spritesArr.begin(), spritesArr.end(), [pInstance, params](uint64_t sprite)
                {
                    auto waitForSprite{ std::make_unique<OnGameObjectDeserialized>(sprite, params.pGameObject->GetScene(),
                    std::vector<std::string>{Component::GetName<SpriteComponent>()},
                    [pInstance](GameObject* pObj)->bool
                    {
                        if (!pObj->HasComponent<SpriteComponent>())
                            return false;

                        SpriteComponent* pSpriteComponent{ pObj->GetComponent<SpriteComponent>() };
                        pInstance->AddSprite(pSpriteComponent);

                        return true;
                    }) };

                    params.outOnDeserialized.push(std::move(waitForSprite));
                });

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion //SpriteAtlasComponent

#pragma region RigidBody2DComponentSerializer
    //=================================
    // RigidBody2DComponent
    //=================================
    struct ColliderInfo
    {
        uint64_t uuid{};
        int type{};
    };

    struct RigdiBody2DBlock
    {
        float restitution{};
        float mass{};
        int collisionMode{};
        int numColliders{};
    };

    RigidBody2DComponentSerializer::RigidBody2DComponentSerializer()
        : Serializable{ Serializable::Create<RigidBody2DComponent>() }
    {
    }
    void RigidBody2DComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        RigidBody2DComponent* pRigidBody{ pComponent->As<RigidBody2DComponent>() };

        RigdiBody2DBlock data{};
        const auto& pColliders{ pRigidBody->GetColliders() };
        data.numColliders = static_cast<int>(pColliders.size());
        data.collisionMode = static_cast<int>(pRigidBody->GetCollisionMode());
        data.mass = pRigidBody->GetMass();
        data.restitution = pRigidBody->GetRestitution();

        std::vector<ColliderInfo> colliders(data.numColliders);
        for (int i{}; i < data.numColliders; ++i)
        {
            colliders[i].uuid = pColliders[i]->GetUUID();
            colliders[i].type = static_cast<int>(pColliders[i]->GetType());
        }

        out.Write(data);

        if (colliders.size() > 0)
            out.WriteArray(&colliders[0], colliders.size());
        else
            out.Write(0);
    }
    Component* RigidBody2DComponentSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            RigdiBody2DBlock rigidBodyData{};
            params.in.Read(rigidBodyData);

            RigidBody2DComponent* pInstance{ params.pGameObject->AddComponent<RigidBody2DComponent>() };
            if (rigidBodyData.collisionMode == 0)
                pInstance->SetStatic();
            else
                pInstance->SetDynamic();
            pInstance->SetMass(rigidBodyData.mass);
            pInstance->SetRestitution(rigidBodyData.restitution);

            std::vector<ColliderInfo> colliders(rigidBodyData.numColliders);
            params.in.ReadArray(colliders);

            std::for_each(colliders.begin(), colliders.end(), [pInstance, params](ColliderInfo colliderInfo)
                {
                    ColliderType type{ static_cast<ColliderType>(colliderInfo.type) };
                    std::string colliderTypeName{};
                    switch (type)
                    {
                    case ColliderType::Box:
                    {
                        colliderTypeName = Component::GetName<BoxCollider2DComponent>();
                    }
                        break;

                    case ColliderType::Circle:
                    {

                    }
                        break;
                    }

                    auto waitForCollider{ std::make_unique<OnGameObjectDeserialized>(colliderInfo.uuid, params.pGameObject->GetScene(),
                    std::vector<std::string>{colliderTypeName},
                    [type, pInstance](GameObject* pObj)->bool
                    {
                            switch (type)
                            {
                            case ColliderType::Box:
                            {
                                if (!pObj->HasComponent<BoxCollider2DComponent>())
                                    return false;

                                BoxCollider2DComponent* pColliderInstance{ pObj->GetComponent<BoxCollider2DComponent>() };
                                pInstance->AddCollider(pColliderInstance);
                            }
                            break;
                            }

                        return true;
                    }) };

                    params.outOnDeserialized.push(std::move(waitForCollider));
                });

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion RigidBody2DComponentSerializer

#pragma region BoxCollider2DComponent
    //=================================
    // BoxCollider2DComponent
    //=================================
    struct ColliderBlock
    {
        int flags{};
        int layer{};
        int ingoreLayer{};
    };

    struct Box2DColliderBlock
    {
        ColliderBlock base{};
        uint64_t quadUuid{};
    };

    BoxCollider2DComponentSerializer::BoxCollider2DComponentSerializer()
        : Serializable(Serializable::Create<BoxCollider2DComponent>())
    {
    }
    void BoxCollider2DComponentSerializer::Serialize(BinaryWriter& out, Component* pComponent) const
    {
        BoxCollider2DComponent* pCollider{ pComponent->As<BoxCollider2DComponent>() };

        Box2DColliderBlock data{};
        data.base.flags = static_cast<int>(pCollider->GetFlags());
        data.base.layer = static_cast<int>(pCollider->GetCollisionLayer());
        data.base.ingoreLayer = static_cast<int>(pCollider->GetCollisionIgnoreLayer());
        data.quadUuid = pCollider->GetShape()->GetUUID();
        out.Write(data);
    }
    Component* BoxCollider2DComponentSerializer::Deserialize(DeserializeParams& params)
    {
        if (params.pGameObject)
        {
            Box2DColliderBlock colliderData{};
            params.in.Read(colliderData);

            BoxCollider2DComponent* pInstance{ params.pGameObject->AddComponent<BoxCollider2DComponent>() };
            pInstance->SetCollisionLayer(static_cast<CollisionLayer>(colliderData.base.layer));
            pInstance->SetCollisionIgnoreLayer(static_cast<CollisionLayer>(colliderData.base.ingoreLayer), true);
            pInstance->SetTrigger(BitFlag::IsSet(static_cast<CollisionFlags>(colliderData.base.flags), CollisionFlags::IsTrigger));

            auto waitForQuad{ std::make_unique<OnGameObjectDeserialized>(colliderData.quadUuid, params.pGameObject->GetScene(),
                std::vector<std::string>{Component::GetName<QuadComponent>()},
                [pInstance, params](GameObject* pObj)->bool
                {
                    if (pObj->HasComponent<QuadComponent>())
                    {
                        pInstance->SetShape(pObj->GetComponent<QuadComponent>());
                        return true;
                    }
                    return false;
                }) };
            params.outOnDeserialized.push(std::move(waitForQuad));

            return pInstance;
        }
        return nullptr;
    }
#pragma endregion BoxCollider2DComponent
}