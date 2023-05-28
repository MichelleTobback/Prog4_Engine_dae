#include "SceneSerializer.h"

#include "Component/TransformComponent.h"
#include "Scene/GameObject.h"
#include "Component/RenderComponent.h"
#include "Component/TextureComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Component/TextComponent.h"
#include "Managers/ResourceManager.h"

#include "Renderer/Texture2D.h"
#include "Renderer/Font.h"

#include "Core/BinaryReader.h"
#include "Core/BitFlag.h"

#include <array>
#include <functional>

namespace dae
{
	std::unique_ptr<OnDeserializedComponentDelegate> SceneSerializer::m_pOnDeserializeComponent
	{ 
		std::make_unique<OnDeserializedComponentDelegate>() 
	};

	struct SceneFileData
	{
		int numGameObjects{};
	};

	struct GameObjectData
	{
		glm::vec3 transform{}; // xy = pos, z = rot
		uint64_t uuid{};
		uint64_t parentUuid{}; //0 = root
		int numComponents{};
	};

	//struct ComponentFileData
	//{
	//	
	//};
	//
	//struct TextureFileData
	//{
	//	
	//};

	struct SpriteFileData
	{
		glm::vec4 source{};
		uint64_t textureUuid{};
	};

	struct SpriteAtlasFileData
	{
		uint64_t textureUuid{};
		int numSprites{};
	};

	struct TextCompFileData
	{
		glm::vec4 color{};
		int fontSize{};
	};

	static void SerializeComponent(Component* pComponent, BinaryWriter& out)
	{
		out.WriteString(typeid(*pComponent).name());

		Serializable* pSerializable{ dynamic_cast<Serializable*>(pComponent) };
		if (pSerializable != nullptr)
		{
			pSerializable->Serialize(out);
		}
	}

	template<typename T, typename Fn>
	static bool SerializeComponent(Component* pComponent, BinaryWriter& out, Fn fn)
	{
		if (pComponent->Is<T>())
		{
			std::string name{ typeid(T).name() };
			//typename
			out.WriteString(name);

			fn(dynamic_cast<T*>(pComponent), out);

			return true;
		}
		return false;
	}
}

//=============================================
// OnGameObjectDeserialized
//=============================================

dae::OnGameObjectDeserialized::OnGameObjectDeserialized(UUID uuid, Scene* pScene, OnGameObjectDeserializedFn fn)
	: m_GameObject{ uuid }
	, m_pScene{ pScene }
	, m_Func{ fn } 
{
}

void dae::OnGameObjectDeserialized::Invoke(const Event& event, Subject* pSubject)
{
	uint64_t uuid{};
	event.GetData(uuid);

	if (uuid == m_GameObject)
	{
		if (m_Func(m_pScene->GetGameObject(uuid), m_pScene))
			pSubject->MarkForDestroy(this);
	}
}

//=============================================
// SceneSerializer
//=============================================

void dae::SceneSerializer::Serialize(Scene* pScene, const std::filesystem::path& path)
{
	m_pScene = pScene;

	std::filesystem::path fullPath{ ResourceManager::GetInstance().GetDataPath() + path.string() };
	if (BinaryWriter output(fullPath); output)
	{
		SceneFileData sceneData{};
		sceneData.numGameObjects = static_cast<int>(pScene->m_Objects.size());
		output.Write(sceneData);
		for (auto& [uuid, pGameObject] : m_pScene->m_Objects)
		{
			SerializeGameObject(pGameObject.get(), output);
		}
	}
}

void dae::SceneSerializer::SerializeGameObject(GameObject* pGameObject, BinaryWriter& out)
{
	if (!BitFlag::IsSet(pGameObject->m_Flags, GameObjectFlag::Serializable))
		return;

	auto& pComponents{ pGameObject->GetComponentSystem().m_Components };
	GameObjectData objectDesc{};

	TransformComponent& trasform{ pGameObject->GetTransform() };
	objectDesc.transform.x = trasform.GetLocalPosition().x;
	objectDesc.transform.y = trasform.GetLocalPosition().y;
	objectDesc.transform.z = trasform.GetLocalRotation();
	objectDesc.uuid = pGameObject->GetUUID();

	//auto& pChildren{ pGameObject->GetChildren() };
	if (!pGameObject->IsRoot())
		objectDesc.parentUuid = pGameObject->GetParent()->GetUUID();
	objectDesc.numComponents = static_cast<int>(pComponents.size());

	out.Write(objectDesc);

	for (auto pComponent : pComponents)
	{
		//TODO - Move component serialization to component using Serializable interface
		// 
		//==================================================
		// TextureComponent
		//==================================================

		if (SerializeComponent<TextureComponent>(pComponent.second.get(), out, [](TextureComponent* pComponent, BinaryWriter& out)
			{
				bool validTexture{ pComponent->HasTexture() };
				Texture2D* pTexture{ (validTexture) ? pComponent->GetTexture().get() : nullptr };
				if (validTexture)
					out.WriteString(pTexture->GetPath());
				else
					out.Write(0);
			})) continue;

		//==================================================
		// SpriteComponent
		//==================================================
		if (SerializeComponent<SpriteComponent>(pComponent.second.get(), out, [](SpriteComponent* pComponent, BinaryWriter& out)
			{
				SpriteFileData spriteData{};
				spriteData.source = pComponent->GetSource();
				auto pTexture{ pComponent->GetTexture() };
				spriteData.textureUuid = (pTexture) ? pTexture->GetUUID() : UUID(0u);

				out.Write(spriteData);
			})) continue;

		//==================================================
		// SpriteAtlasComponent
		//==================================================
		if (SerializeComponent<SpriteAtlasComponent>(pComponent.second.get(), out, [](SpriteAtlasComponent* pComponent, BinaryWriter& out)
			{
				SpriteAtlasFileData data{};
				auto& pSprites{ pComponent->GetSprites() };
				data.numSprites = static_cast<int>(pSprites.size());
				auto pTexture{ &pComponent->GetTexture() };
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

			})) continue;

		//==================================================
		// TextComponent
		//==================================================
		if (SerializeComponent<TextComponent>(pComponent.second.get(), out, [](TextComponent* pComponent, BinaryWriter& out)
			{
				TextCompFileData txtData{};
				auto pFont{ pComponent->GetFont() };
				txtData.fontSize = pFont->GetSize();
				txtData.color = pComponent->GetColor();

				out.Write(txtData);
				out.WriteString(pFont->GetPath());
				out.WriteString(pComponent->GetText());
			})) continue;

		//==================================================
		// TextureRendererComponent
		//==================================================
		if (SerializeComponent<TextureRenderComponent>(pComponent.second.get(), out, [](TextureRenderComponent* pComponent, BinaryWriter& out)
			{
				uint64_t textureUuid{ (pComponent->GetTexture()) ? pComponent->GetTexture()->GetUUID() : UUID(0u) };

				out.Write(textureUuid);
			})) continue;

		//==================================================
		// SpriteRenderComponent
		//==================================================
		if (SerializeComponent<SpriteRenderComponent>(pComponent.second.get(), out, [](SpriteRenderComponent* pComponent, BinaryWriter& out)
			{
				uint64_t spriteUuid{ (pComponent->GetSprite()) ? pComponent->GetSprite()->GetUUID() : UUID(0u) };

				out.Write(spriteUuid);
			})) continue;

		SerializeComponent(pComponent.second.get(), out);
	}
}

dae::GameObject* dae::SceneSerializer::GetDeserializedGameObject(UUID uuid, UUID parent)
{
	if (!uuid.IsValid())
		return nullptr;

	GameObject* pGameObject{m_pScene->GetGameObject(uuid)};
	if (pGameObject == nullptr)
	{
		GameObject* pParentInstance{ GetDeserializedGameObject(parent, 0u) };
		pGameObject = m_pScene->Instantiate(uuid, pParentInstance);
	}
	else if (pGameObject->IsRoot() && parent.IsValid())
	{
		GameObject* pParentInstance{ GetDeserializedGameObject(parent, 0u) };
		pGameObject->AttachToGameObject(pParentInstance);
	}
	return pGameObject;
}

void dae::SceneSerializer::Deserialize(Scene* pScene, const std::filesystem::path& path)
{
	m_pScene = pScene;

	std::filesystem::path fullPath{ ResourceManager::GetInstance().GetDataPath() + path.string() };
	if (BinaryReader input(fullPath); input)
	{
		SceneFileData sceneData{};
		input.Read(sceneData);

		for (int i{}; i < sceneData.numGameObjects; ++i)
		{
			GameObjectData objectDesc{};
			input.Read(objectDesc);

			GameObject* pInstance{ GetDeserializedGameObject(objectDesc.uuid, objectDesc.parentUuid) };

			if (pInstance == nullptr)
				continue;

			pInstance->GetTransform().Translate({ objectDesc.transform.x, objectDesc.transform.y, 0.f });
			if (std::abs(objectDesc.transform.z) > 0.001f)
				pInstance->GetTransform().Rotate(objectDesc.transform.z);

			for (int compIdx{}; compIdx < objectDesc.numComponents; ++compIdx)
			{
				//ComponentFileData componentFileData{};
				//input.Read(componentFileData);

				std::string componentName{};
				input.ReadString(componentName);

				if (!componentName.empty())
				{
					//==================================================
					// TextureComponent reading
					//==================================================
					if (componentName._Equal(typeid(TextureComponent).name()))
					{
						std::string texPath{};
						if (!input.ReadString(texPath).empty())
						{
							pInstance->AddComponent<TextureComponent>()->SetTexture(texPath);
						}
					}
					//==================================================
					// SpriteComponent reading
					//==================================================
					else if (componentName._Equal(typeid(SpriteComponent).name()))
					{
						SpriteFileData spriteData{};
						input.Read(spriteData);

						DeserializeComponentOnRefInstantiated(spriteData.textureUuid, [pInstance, spriteData](GameObject* pComponentOwner, Scene*)
							{
								if (!pComponentOwner->HasComponent<TextureComponent>())
									return false;

								TextureComponent* textureComponent{ pComponentOwner->GetComponent<TextureComponent>() };
								pInstance->AddComponent<SpriteComponent>(textureComponent, spriteData.source);

								return true;
							});
					}
					//==================================================
					// SpriteAtlasComponent reading
					//==================================================
					else if (componentName._Equal(typeid(SpriteAtlasComponent).name()))
					{
						SpriteAtlasFileData saData{};
						input.Read(saData);

						std::vector<UUID> saSprites{};
						input.ReadArray(saSprites);

						//Wait on referenced TextureComponent
						DeserializeComponentOnRefInstantiated(saData.textureUuid, [this, pInstance, saData, saSprites](GameObject* pComponentOwner, Scene*)->bool
							{
								if (!pComponentOwner->HasComponent<TextureComponent>())
									return false;

								TextureComponent* textureComponent{ pComponentOwner->GetComponent<TextureComponent>() };
								auto pSpriteAtlasComponent{ pInstance->AddComponent<SpriteAtlasComponent>(textureComponent) };

								for (UUID sprite : saSprites)
								{
									//Wait on referenced Sprite
									DeserializeComponentOnRefInstantiated(uint64_t(sprite), [pInstance, pSpriteAtlasComponent](GameObject* pOwner, Scene*)->bool
										{
											if (!pOwner->HasComponent<SpriteComponent>())
												return false;

											SpriteComponent* spriteComponent{ pOwner->GetComponent<SpriteComponent>() };
											pSpriteAtlasComponent->AddSprite(spriteComponent);

											return true;
										});
								}
								return true;
							});
					}
					//==================================================
					// TextComponent reading
					//==================================================
					else if (componentName._Equal(typeid(TextComponent).name()))
					{
						TextCompFileData tcData{};
						input.Read(tcData);

						std::string fontPath{};
						input.ReadString(fontPath);
						std::string text{};
						input.ReadString(text);

						auto pFont{ ResourceManager::GetInstance().LoadFont(fontPath, static_cast<unsigned int>(tcData.fontSize)) };
						pInstance->AddComponent<TextComponent>(text, std::move(pFont), tcData.color);
					}

					//==================================================
					// TextureRenderComponent reading
					//==================================================
					else if (componentName._Equal(typeid(TextureRenderComponent).name()))
					{
						uint64_t textureUuid{};
						input.Read(textureUuid);

						DeserializeComponentOnRefInstantiated(textureUuid, [pInstance](GameObject* pComponentOwner, Scene*)
							{
								if (!pComponentOwner->HasComponent<TextureComponent>())
									return false;

								TextureComponent* textureComponent{ pComponentOwner->GetComponent<TextureComponent>() };
								pInstance->AddComponent<TextureRenderComponent>(textureComponent);

								return true;
							});
					}
					//==================================================
					// SpriteRenderComponent reading
					//==================================================
					else if (componentName._Equal(typeid(SpriteRenderComponent).name()))
					{
						uint64_t spriteUuid{};
						input.Read(spriteUuid);

						DeserializeComponentOnRefInstantiated(spriteUuid, [pInstance](GameObject* pComponentOwner, Scene*)
							{
								if (!pComponentOwner->HasComponent<SpriteComponent>())
									return false;

								SpriteComponent* textureComponent{ pComponentOwner->GetComponent<SpriteComponent>() };
								pInstance->AddComponent<SpriteRenderComponent>(textureComponent);

								return true;
							});
					}
					else
					{
						DeserializeParams params{input, pInstance, componentName, m_OnDeserialized };
						m_pOnDeserializeComponent->Invoke(params);
					}
				}
			}

			Event event{};
			event.SetData(objectDesc.uuid);
			m_GameObjectDeserialized.Invoke(event);
		}
		m_GameObjectDeserialized.Clear();
		m_OnDeserialized.clear();
	}
}

dae::OnDeserializedComponentDelegate* dae::SceneSerializer::GetOnDeserializedComponentDelegate()
{
	return m_pOnDeserializeComponent.get();
}
