#include "SceneSerializer.h"

#include "Component/TransformComponent.h"
#include "Scene/GameObject.h"
#include "Component/RenderComponent.h"
#include "Component/TextureComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Component/TextComponent.h"
#include "Managers/ResourceManager.h"
#include "ComponentFactory.h"
#include "Component/SerializableComponents.h"

#include "Renderer/Texture2D.h"
#include "Renderer/Font.h"

#include "Core/BinaryReader.h"
#include "Core/BitFlag.h"

#include <array>
#include <functional>

namespace dae
{
	struct SceneFileData
	{
		int32_t numGameObjects{};
	};

	struct GameObjectData
	{
		Vec3 transform{}; // xy = pos, z = rot
		uint64_t uuid{};
		uint64_t parentUuid{}; //0 = root
		int32_t numComponents{};
	};

	struct SpriteFileData
	{
		Vec4 source{};
		uint64_t textureUuid{};
	};

	struct SpriteAtlasFileData
	{
		uint64_t textureUuid{};
		int32_t numSprites{};
	};

	static void SerializeComponent(Component* pComponent, BinaryWriter& out, ComponentFactory& factory)
	{
		ComponentID id{ 0u };
		if (factory.IsRegistered(pComponent->GetName()))
		{
			id = factory.GetID(pComponent->GetName());
			Serializable* pSerializable{ factory.GetComponentSerializer(id) };
			if (pSerializable != nullptr && pSerializable->CanBeSerialized())
			{
				out.Write(id);
				pSerializable->Serialize(out, pComponent);
				return;
			}
		}
		out.Write(id);
		return;
	}

	static void SerializeComponentTypes(BinaryWriter& out, ComponentFactory& factory)
	{
		out.Write(factory.GetRegisteredCount());
		factory.ForEachRegisteredComponent([&](const SerializableType& type, Serializable*)
			{
				out.Write(type.id);
				out.WriteString(type.componentName);
			});
	}

	static void DeserializeComponentTypes(BinaryReader& in, ComponentFactory& factory, std::unordered_map<ComponentID, ComponentID>& sceneIdToCurrentId)
	{
		uint64_t componentCount{};
		in.Read(componentCount);

		for (size_t i{}; i < componentCount; ++i)
		{
			ComponentID id{};
			in.Read(id);
			std::string name{};
			in.ReadString(name);
			sceneIdToCurrentId.emplace(id, factory.GetID(name));
		}
	}
}

//=============================================
// OnGameObjectDeserialized
//=============================================

dae::OnGameObjectDeserialized::OnGameObjectDeserialized(UUID uuid, Scene* pScene, const std::vector<std::string>& components, OnGameObjectDeserializedFn fn)
	: m_GameObject{ uuid }
	, m_pScene{ pScene }
	, m_ComponentsToWaitFor{components}
	, m_Func{ fn } 
{
}

void dae::OnGameObjectDeserialized::Invoke(const Event& event, Subject* pSubject)
{
	uint64_t uuid{};
	event.GetData(uuid);

	GameObject* pObject{ m_pScene->GetGameObject(m_GameObject) };
	if (pObject)
	{
		for (const std::string& c : m_ComponentsToWaitFor)
		{
			if (!pObject->HasComponent(c))
				return;
		}

		if (m_Func(pObject))
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

		ComponentFactory& componentFactory{ *SceneManager::GetInstance().GetComponentFactory() };
		SerializeComponentTypes(output, componentFactory);

		for (auto& [uuid, pGameObject] : m_pScene->m_Objects)
		{
			SerializeGameObject(pGameObject.get(), output, componentFactory);
		}
		output.SetPos(0);
		sceneData.numGameObjects = m_SerializableObjects;
		output.Write(sceneData);
	}
}

void dae::SceneSerializer::SerializeGameObject(GameObject* pGameObject, BinaryWriter& out, ComponentFactory& factory)
{
	if (!BitFlag::IsSet(pGameObject->m_Flags, GameObjectFlag::Serializable))
		return;

	auto& components{ pGameObject->GetComponentSystem() };

	GameObjectData objectDesc{};

	TransformComponent& trasform{ pGameObject->GetTransform() };
	objectDesc.transform.x = trasform.GetLocalPosition().x;
	objectDesc.transform.y = trasform.GetLocalPosition().y;
	objectDesc.transform.z = trasform.GetLocalRotation();
	objectDesc.uuid = pGameObject->GetUUID();

	if (!pGameObject->IsRoot())
		objectDesc.parentUuid = pGameObject->GetParent()->GetUUID();
	objectDesc.numComponents = static_cast<int>(components.Count());

	out.Write(objectDesc);

	components.ForEach([&](Component* pComponent)
		{
			dae::SerializeComponent(pComponent, out, factory);
		});

	++m_SerializableObjects;
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

void dae::SceneSerializer::RegisterEngineComponents(ComponentFactory& factory)
{
	//resources
	factory.RegisterComponent<QuadSerializer>();
	factory.RegisterComponent<SpriteComponentSerializer>();
	factory.RegisterComponent<TextureSerializer>();
	factory.RegisterComponent<TextComponentSerializer>();
	factory.RegisterComponent<SpriteAtlasComponentSerializer>();
	factory.RegisterComponent<TagComponentSerializer>();

	//renderer
	factory.RegisterComponent<QuadRendererSerializer>();
	factory.RegisterComponent<SpriteRendererSerializer>();
	factory.RegisterComponent<TextureRendererSerializer>();

	//physics
	factory.RegisterComponent<RigidBody2DComponentSerializer>();
	factory.RegisterComponent<BoxCollider2DComponentSerializer>();
}

void dae::SceneSerializer::Deserialize(Scene* pScene, const std::filesystem::path& path)
{
	m_pScene = pScene;

	std::filesystem::path fullPath{ ResourceManager::GetInstance().GetDataPath() + path.string() };
	if (BinaryReader input(fullPath); input)
	{
		SceneFileData sceneData{};
		input.Read(sceneData);

		std::queue<std::unique_ptr<OnGameObjectDeserialized>> observersToAdd{};
		std::unordered_map<ComponentID, ComponentID> sceneIdToCurrentId{};
		ComponentFactory& componentFactory{ *SceneManager::GetInstance().GetComponentFactory() };

		DeserializeComponentTypes(input, componentFactory, sceneIdToCurrentId);

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

				ComponentID componentID{0u};
				input.Read(componentID);

				if (sceneIdToCurrentId.find(componentID) == sceneIdToCurrentId.end())
					continue;

				ComponentID currentId{ sceneIdToCurrentId[componentID] };
				if (componentFactory.IsRegistered(currentId))
				{
					DeserializeParams params{ input, pInstance, observersToAdd };
					componentFactory.InstantiateComponent(currentId, params);

					while (!observersToAdd.empty())
					{
						if (observersToAdd.front()->GetGameObject() != 0u)
						{
							m_OnDeserialized.push_back(std::move(observersToAdd.front()));
							m_GameObjectDeserialized.AddObserver(m_OnDeserialized.back().get());
						}
						observersToAdd.pop();
					}

					Event event{};
					event.SetData(objectDesc.uuid);
					m_GameObjectDeserialized.Invoke(event);
				}
			}
		}
		m_GameObjectDeserialized.Clear();
		m_OnDeserialized.clear();
	}
}
