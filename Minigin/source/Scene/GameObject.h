#pragma once
#include <memory>

#include "Component/UUIDComponent.h"
#include "Component/ComponentSystem.h"
#include "Component/TransformComponent.h"
#include "Core/UUID.h"
#include "Core/Delegate.h"
#include "Component/TagComponent.h"

namespace dae
{
	typedef uint32_t GameObjectID;

	enum class GameObjectFlag
	{
		None = 0,
		Serializable = 1,
		Active = 2
	};

	class Scene;
	class GameObject final
	{
	public:
		GameObject(Scene* pScene);
		GameObject(Scene* pScene, UUID uuid);
		virtual ~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Awake();
		void Sleep();
		void OnDestroy();
		void Update();
		void FixedUpdate();
		void LateUpdate();

		void SetActive(bool active);
		bool IsActive() const;

		void BroadcastComponentMessage(const ComponentMessage& msg);

		void AttachToGameObject(GameObject* pParent, bool keepWorld = false);
		void DetachGameObject(GameObject* pChild);
		GameObject* GetRoot();

		Scene* GetScene() { return m_pScene; }

		inline bool IsValid() const { return m_IsValid; }
		bool IsRoot() const;

		template <typename T, typename ... TArgs>
		T* AddComponent(TArgs&& ... args)
		{
			T* pComponent{ m_ComponentSystem->AddComponent<T>(this, std::forward<TArgs>(args)...) };
			return pComponent;
		}

		template <typename T>
		bool RemoveComponent()
		{
			return m_ComponentSystem->RemoveComponent<T>();
		}

		template <typename T>
		const T* GetComponent() const
		{
			return m_ComponentSystem->GetComponent<T>();
		}

		template <typename T>
		T* GetComponent()
		{
			return m_ComponentSystem->GetComponent<T>();
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_ComponentSystem->HasComponent<T>();
		}
		bool HasComponent(const std::string& typeName) const;

		bool IsFlagSet(GameObjectFlag flag) const;
		void Destroy();

		inline GameObject* GetParent() const { return m_pParent; }
		inline const std::vector<GameObject*>& GetChildren() const { return m_pChildren; }
		inline TransformComponent& GetTransform() { return *m_pTransformComponent; }
		inline UUID GetUUID() const { return m_pUUID->GetUUID(); }
		inline const ComponentSystem& GetComponentSystem() const { return *m_ComponentSystem; }

		using GameObjectDelegate = Delegate<void(GameObject*)>;
		inline GameObjectDelegate& GetOnChildAttachedDelegate() { return *m_pOnChildAttached; }
		inline GameObjectDelegate& GetOnDestroyed() { return *m_pOnDestroyed; }

		inline TagComponent* GetTag() const { return m_pTag; }
		void AddTag(const std::string& tag, bool includeChildren = false);
		bool HasTag(const std::string& tag);

	protected:
		void SetFlag(GameObjectFlag flag, bool set);

	private:
		friend class SceneSerializer;
		inline void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);

		std::unique_ptr<ComponentSystem> m_ComponentSystem;
		bool m_IsValid{ true };

		GameObject* m_pParent{nullptr};
		std::vector<GameObject*> m_pChildren{};

		TransformComponent* m_pTransformComponent{ nullptr };
		UUIDComponent* m_pUUID{ nullptr };
		Scene* m_pScene;
		std::unique_ptr<GameObjectDelegate> m_pOnChildAttached{nullptr};
		std::unique_ptr<GameObjectDelegate> m_pOnDestroyed{ nullptr };

		GameObjectFlag m_Flags;
		TagComponent* m_pTag{ nullptr };
	};
}
