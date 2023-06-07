#pragma once
#include "Core/Observer.h"
#include "Core/BinaryReader.h"
#include "Core/UUID.h"
#include "Component/Component.h"
#include <functional>
#include <queue>


namespace dae
{
	class GameObject;
	class Scene;
	using OnGameObjectDeserializedFn = std::function<bool(GameObject*)>;
	class OnGameObjectDeserialized final : public Observer
	{
	public:
		OnGameObjectDeserialized(UUID uuid, Scene* pScene, const std::vector<std::string>& components, OnGameObjectDeserializedFn fn);
		virtual ~OnGameObjectDeserialized() = default;

		virtual void Invoke(const Event& event, Subject* pSubject) override;
		UUID GetGameObject() { return m_GameObject; }

	private:
		UUID m_GameObject{};
		Scene* m_pScene;
		std::vector<std::string> m_ComponentsToWaitFor;
		OnGameObjectDeserializedFn m_Func;
	};

	using ComponentID = size_t;

	struct SerializableType
	{
		std::string componentName{};
		ComponentID id{};

		bool operator==(const SerializableType& other) const
		{
			return *this == other.id;
		}
		bool operator!=(const SerializableType& other) const
		{
			return !(*this == other);
		}
		bool operator==(const std::string& otherName) const
		{
			return componentName._Equal(otherName);
		}
		bool operator!=(const std::string& otherName) const
		{
			return !(*this == otherName);
		}
		bool operator==(ComponentID otherId) const
		{
			return id == otherId;
		}
		bool operator!=(ComponentID otherId) const
		{
			return id != otherId;
		}
	};

	struct DeserializeParams
	{
		BinaryReader& in; 
		GameObject* pGameObject; 
		std::queue<std::unique_ptr<OnGameObjectDeserialized>>& outOnDeserialized;
	};

	class Serializable
	{
	public:
		
		virtual ~Serializable() = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const = 0;
		virtual Component* Deserialize(DeserializeParams& params) = 0;
		virtual bool CanBeSerialized() const { return true; }

		const SerializableType& GetTypeInfo() const { return m_ComponentTypeInfo; }

	protected:
		Serializable(const SerializableType& typeInfo) : m_ComponentTypeInfo{ typeInfo } {}
		template<typename T>
		static SerializableType Create();

	private:
		friend class ComponentFactory;
		SerializableType m_ComponentTypeInfo;
	};

	template<typename T>
	SerializableType Serializable::Create()
	{
		SerializableType typeInfo{};
		typeInfo.componentName = Component::GetName<T>();
		typeInfo.id = typeid(T).hash_code();
		return typeInfo;
	}
}