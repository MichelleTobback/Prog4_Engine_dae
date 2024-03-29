#include "ButtonComponent.h"
#include "Component/QuadComponent.h"
#include "Scene/GameObject.h"
#include "Input/Input.h"
#include "Core/Math.h"

#include "Renderer/Renderer.h"
#include "Managers/ServiceLocator.h"
#include "Component/CameraComponent.h"
#include "Component/InputHandlerComponent.h"

dae::ButtonComponent::ButtonComponent(GameObject* pOwner, const glm::vec2& size)
	: Component(pOwner)
{
	BitFlag::Set(m_Flags, UIButtonFlag::DefaultState, true);
	m_pOnPressedDelegate = std::make_unique<ButtonDelegate>();
	m_pOnReleasedDelegate = std::make_unique<ButtonDelegate>();
	m_pOnHoveredDelegate = std::make_unique<ButtonDelegate>();
	m_pQuad = GetOwner()->AddComponent<QuadComponent>(size, m_DefaultLayout.color);
	m_pQuadRenderer = GetOwner()->AddComponent<QuadRendererComponent>(m_pQuad);
	m_pSpriteRenderer = GetOwner()->AddComponent<SpriteRenderComponent>(m_DefaultLayout.pSprite);

	auto& input{ GetOwner()->AddComponent<InputHandlerComponent>()->GetHandler() };
	{
		auto& commandBinding{ input.AddAxisBinding(0) };
		dae::InputCommand::ICDevices binding{};
		BitFlag::Set(binding.flags, InputCommand::ICFlag::MouseMoved, true);
		commandBinding.Add({}, binding);
		input.BindAxisCommand(0, InputCommand::AxisCommand::Create(this, &ButtonComponent::OnMouseMoved));
	}
	{
		auto& commandBinding{ input.AddActionBinding(1) };
		dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
		BitFlag::Set(binding.flags, InputCommand::ICFlag::MouseButton, true);
		binding.mouse.button = Mouse::MouseButton::Left;
		binding.mouse.state = Mouse::MouseButtonState::Pressed;
		input.BindActionCommand(1, ActionCommand::Create(this, &ButtonComponent::OnPressed));
	}
	{
		auto& commandBinding{ input.AddActionBinding(2) };
		dae::InputCommand::ICDevices& binding{ commandBinding.deviceBinding };
		BitFlag::Set(binding.flags, InputCommand::ICFlag::MouseButton, true);
		binding.mouse.button = Mouse::MouseButton::Left;
		binding.mouse.state = Mouse::MouseButtonState::Released;
		input.BindActionCommand(2, ActionCommand::Create(this, &ButtonComponent::OnReleased));
	}
}

void dae::ButtonComponent::Awake()
{
	//Input::InputActionCommand actionCommand{ ActionCommand::Create(this, &ButtonComponent::OnPressed) };
	//BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::MouseButton, true);
	//actionCommand.Mouse.button = Mouse::MouseButton::Left;
	//actionCommand.Mouse.state = Mouse::MouseButtonState::Pressed;
	//Input::GetInstance().AddActionCommand(actionCommand);
	//
	//actionCommand = ActionCommand::Create(this, &ButtonComponent::OnReleased);
	//actionCommand.Mouse.state = Mouse::MouseButtonState::Released;
	//Input::GetInstance().AddActionCommand(actionCommand);
	//
	//Input::InputAxisCommand axisCommand{ ValueCommand<glm::vec2>::Create(&ButtonComponent::OnMouseMoved, this) };
	//BitFlag::Set(axisCommand.flags, Input::InputCommandFlag::MouseMoved, true);
	//Input::GetInstance().AddAxisCommand(axisCommand);
}

void dae::ButtonComponent::Update()
{
	
}

void dae::ButtonComponent::SetLayout(const UIButtonLayout& layout, UIButtonFlag state)
{
	const bool isCurrentState{ BitFlag::IsSet(m_Flags, state) };
	switch (state)
	{
	case UIButtonFlag::Hovered:
	{
		m_HoveredLayout = layout;

		if (isCurrentState)
		{
			if (m_HoveredLayout.pSprite != nullptr)
			{
				m_pSpriteRenderer->SetSpriteComponent(m_HoveredLayout.pSprite);
			}

			m_pQuad->SetColor(m_HoveredLayout.color);
		}
	}
		break;
	case UIButtonFlag::Selected:
	{
		m_SelectedLayout = layout;
		if (isCurrentState)
		{
			if (m_SelectedLayout.pSprite != nullptr)
			{
				m_pSpriteRenderer->SetSpriteComponent(m_SelectedLayout.pSprite);
			}

			m_pQuad->SetColor(m_SelectedLayout.color);
		}
	}
		break;
	case UIButtonFlag::DefaultState:
	{
		m_DefaultLayout = layout;
		if (isCurrentState)
		{
			if (m_DefaultLayout.pSprite != nullptr)
			{
				m_pSpriteRenderer->SetSpriteComponent(m_DefaultLayout.pSprite);
			}

			m_pQuad->SetColor(m_DefaultLayout.color);
		}
	}
		break;
	}

	
}

const dae::UIButtonLayout& dae::ButtonComponent::GetLayout(UIButtonFlag state) const
{
	switch (state)
	{
	case UIButtonFlag::Hovered:
		return m_HoveredLayout;
		break;
	case UIButtonFlag::Selected:
		return m_SelectedLayout;
		break;
	}
	return m_DefaultLayout;
}

void dae::ButtonComponent::EnableToggle(bool enable)
{
	m_Flags = (enable) 
		? m_Flags = static_cast<UIButtonFlag>(static_cast<int>(m_Flags) | static_cast<int>(UIButtonFlag::Toggle))
		: m_Flags = static_cast<UIButtonFlag>(static_cast<int>(m_Flags) & ~static_cast<int>(UIButtonFlag::Toggle));
}

bool dae::ButtonComponent::IsHovered() const
{
	return IsFlagSet(UIButtonFlag::Hovered);
}

bool dae::ButtonComponent::IsSelected() const
{
	return IsFlagSet(UIButtonFlag::Selected);
}

bool dae::ButtonComponent::IsFlagSet(UIButtonFlag flag) const
{
	return (static_cast<int>(m_Flags) & static_cast<int>(flag));
}

void dae::ButtonComponent::Select()
{
	BitFlag::Set(m_Flags, UIButtonFlag::Selected, true);
	SetLayoutActive(UIButtonFlag::Selected);

	if (m_pOnPressedDelegate)
		m_pOnPressedDelegate->Invoke();
}

void dae::ButtonComponent::Deselect()
{
	BitFlag::Set(m_Flags, UIButtonFlag::Selected, false);
	SetLayoutActive(UIButtonFlag::DefaultState);

	if (m_pOnReleasedDelegate)
		m_pOnReleasedDelegate->Invoke();
}

void dae::ButtonComponent::OnPressed()
{
	if (BitFlag::IsSet(m_Flags, UIButtonFlag::Hovered))
	{
		bool toggle{ BitFlag::IsSet(m_Flags, UIButtonFlag::Toggle) };
		bool selected{ BitFlag::IsSet(m_Flags, UIButtonFlag::Selected) };

		if (!toggle || toggle && !selected)
		{
			Select();
		}
		else if (toggle && selected)
		{
			Deselect();
		}
	}
}

void dae::ButtonComponent::OnReleased()
{
	if (BitFlag::IsSet(m_Flags, UIButtonFlag::Hovered) && 
		!BitFlag::IsSet(m_Flags, UIButtonFlag::Toggle))
	{
		Deselect();
	}
}

void dae::ButtonComponent::OnMouseMoved(const glm::vec2&)
{
	CameraComponent* pCamera{ Renderer::GetInstance().GetActiveCamera() };
	glm::vec2 camScale{ 1.f, 1.f };
	glm::vec3 camPos{};
	if (pCamera)
	{
		auto& window{ ServiceLocator::GetWindow() };
		const auto& camSize{ pCamera->GetSize() };
		camScale.x = window.GetWidth() / camSize.x;
		camScale.y = window.GetHeight() / camSize.y;
		camPos = pCamera->GetTransform().GetWorldPosition();
	}

	glm::vec2 mousePos{ Input::GetInstance().GetMouse()->GetMousePos() };
	const glm::vec2& size{ m_pQuad->GetSize() * camScale };
	glm::vec3 worldPos{ GetOwner()->GetTransform().GetWorldPosition() * glm::vec3{ camScale.x, camScale.y, 1.f } + camPos };
	//worldPos.x -= size.x * 0.5f;
	//worldPos.y -= size.y * 0.5f;
	bool isHovered{ Math::isPointInsideRect(mousePos, {worldPos.x, worldPos.y}, size)};

	if (isHovered && !BitFlag::IsSet(m_Flags, UIButtonFlag::Hovered))
	{
		BitFlag::Set(m_Flags, UIButtonFlag::Hovered, true);
		OnMouseEnter();
	}

	else if (!isHovered && BitFlag::IsSet(m_Flags, UIButtonFlag::Hovered))
	{
		BitFlag::Set(m_Flags, UIButtonFlag::Hovered, false);
		OnMouseExit();
	}
}

void dae::ButtonComponent::OnMouseEnter()
{
	SetLayoutActive(UIButtonFlag::Hovered);

	m_pOnHoveredDelegate->Invoke();
}

void dae::ButtonComponent::OnMouseExit()
{
	bool toggle{ BitFlag::IsSet(m_Flags, UIButtonFlag::Toggle) };
	bool selected{ BitFlag::IsSet(m_Flags, UIButtonFlag::Selected) };
	if (!toggle || toggle && !selected)
		SetLayoutActive(UIButtonFlag::DefaultState);
	else if (toggle && selected)
		SetLayoutActive(UIButtonFlag::Selected);
}

void dae::ButtonComponent::SetLayoutActive(UIButtonFlag state)
{
	const UIButtonLayout& layout{ GetLayout(state) };
	if (layout.pSprite != nullptr)
	{
		m_pSpriteRenderer->SetSpriteComponent(layout.pSprite);
	}

	m_pQuad->SetColor(layout.color);
}
