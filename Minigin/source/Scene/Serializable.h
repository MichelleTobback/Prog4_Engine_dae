#pragma once
#include "Core/Observer.h"
#include "Core/BinaryReader.h"
#include "Core/Delegate.h"
#include "Core/UUID.h"
#include <functional>


namespace dae
{
	class Serializable
	{
	public:
		Serializable() = default;
		virtual ~Serializable() = default;

		virtual void Serialize(BinaryWriter& out) const = 0;
	};

	class GameObject;
	class Scene;
	using OnGameObjectDeserializedFn = std::function<bool(GameObject*, Scene*)>;
	class OnGameObjectDeserialized final : public Observer
	{
	public:
		OnGameObjectDeserialized(UUID uuid, Scene* pScene, OnGameObjectDeserializedFn fn);
		virtual ~OnGameObjectDeserialized() = default;

		virtual void Invoke(const Event& event, Subject* pSubject) override;
		UUID GetGameObject() { return m_GameObject; }

	private:
		UUID m_GameObject{};
		Scene* m_pScene;
		OnGameObjectDeserializedFn m_Func;
	};

	struct DeserializeParams
	{
		BinaryReader& in; 
		GameObject* pGameObject; 
		const std::string& componentName; 
		std::list<OnGameObjectDeserialized>& outOnDeserialized;
		bool succes{ false };
	};
	using OnDeserializedComponentDelegate = Delegate<void(DeserializeParams&)>;
}