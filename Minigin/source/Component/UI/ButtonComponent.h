#pragma once
#include "Component/Component.h"
#include "Component/RenderComponent.h"
#include "Core/Delegate.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace dae
{
	class SpriteComponent;
	struct UIButtonLayout
	{
		glm::vec4 color;
		SpriteComponent* pSprite{ nullptr };
	};

	constexpr uint32_t UI_NUM_BUTTON_STATES = 3;
	enum class UIButtonFlag
	{
		None = 0, 
		Hovered = 1, Selected = 2, DefaultState = 4,
		Toggle = 8, Background = 16, Border = 32
	};

	class QuadComponent;
	class ButtonComponent final : public Component
	{
	public:
		using ButtonDelegate = Delegate<void()>;

		ButtonComponent(GameObject* pOwner, const glm::vec2& size);
		virtual ~ButtonComponent() {}

		ButtonComponent(const ButtonComponent& other) = delete;
		ButtonComponent(ButtonComponent&& other) = delete;
		ButtonComponent& operator=(const ButtonComponent& other) = delete;
		ButtonComponent& operator=(ButtonComponent&& other) = delete;

		virtual void Awake() override;
		virtual void Update() override;

		void SetLayout(const UIButtonLayout& layout, UIButtonFlag state = UIButtonFlag::DefaultState);
		const UIButtonLayout& GetLayout(UIButtonFlag state) const;

		void EnableToggle(bool enable);
		bool IsHovered() const;
		bool IsSelected() const;

		void Select();
		void Deselect();

		QuadRendererComponent* GetQuadRenderer() const { return m_pQuadRenderer; }
		SpriteRenderComponent* GetSpriteRenderer() const { return m_pSpriteRenderer; }

		ButtonDelegate& GetOnPressedDelegate() { return *m_pOnPressedDelegate; }
		ButtonDelegate& GetOnReleasedDelegate() { return *m_pOnReleasedDelegate; }
		ButtonDelegate& GetOnHoveredDelegate() { return *m_pOnHoveredDelegate; }

	private:
		QuadComponent* m_pQuad{ nullptr };
		QuadRendererComponent* m_pQuadRenderer{ nullptr };
		SpriteRenderComponent* m_pSpriteRenderer{ nullptr };
		UIButtonFlag m_Flags{ UIButtonFlag::Toggle };

		UIButtonLayout m_DefaultLayout{};
		UIButtonLayout m_HoveredLayout{};
		UIButtonLayout m_SelectedLayout{};

		std::unique_ptr<ButtonDelegate> m_pOnPressedDelegate{ nullptr };
		std::unique_ptr<ButtonDelegate> m_pOnReleasedDelegate{ nullptr };
		std::unique_ptr<ButtonDelegate> m_pOnHoveredDelegate{ nullptr };

		bool IsFlagSet(UIButtonFlag flag) const;
		void OnPressed();
		void OnReleased();
		void OnMouseMoved(const glm::vec2& deltaMouse);
		void OnMouseEnter();
		void OnMouseExit();
		void SetLayoutActive(UIButtonFlag state);
	};
}