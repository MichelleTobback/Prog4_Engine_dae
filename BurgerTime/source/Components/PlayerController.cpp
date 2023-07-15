#include "PlayerController.h"
#include "Input/Input.h"
#include "Scene/GameObject.h"
#include <memory>
#include "Scene/Scene.h"
#include "Prefabs.h" // LADDER_COLLISION_LAYER

dae::PlayerController::PlayerController(GameObject* pOwner, CharacterController2D* pCharactarerController, int controllerIndex)
	: Component{ pOwner }
	, m_pCharacterController{ pCharactarerController }
	, m_ControllerIndex{ controllerIndex }
{
	//m_pTank->GetHealth().GetOnDeath() += std::bind(&PlayerController::OnDeath, this);
	const float tileSize{ 16.f };

	m_pLadderCollider = GetScene()->Instantiate(GetOwner(), glm::vec3{tileSize * 0.5f - 2.f, -2.f, 0.f})->AddComponent<BoxCollider2DComponent>();
	m_pLadderCollider->SetShape(m_pLadderCollider->GetOwner()->AddComponent<QuadComponent>(glm::vec2{ 4.f, tileSize * 0.3f }));
	m_pLadderCollider->SetCollisionLayer(PLAYER_COLLISION_LAYER);
	pCharactarerController->GetRigidBody()->AddCollider(m_pLadderCollider);

	m_pCharacterController->GetRigidBody()->GetOnBeginOverlap() += std::bind(&PlayerController::OnOverlap, this, std::placeholders::_1);
	m_pCharacterController->GetRigidBody()->GetOnEndOverlap() += std::bind(&PlayerController::OnEndOverlap, this, std::placeholders::_1);

	//Movement
	{
		glm::vec2 dir{ -1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::A;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 1.f, 0.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::D;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, 1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::S;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
	{
		glm::vec2 dir{ 0.f, -1.f };
		Input::InputValueCommand<glm::vec2> actionCommand{ ValueCommand<glm::vec2>::Create(&PlayerController::Move, this) };
		BitFlag::Set(actionCommand.flags, Input::InputCommandFlag::Keyboard, true);
		actionCommand.value = dir;
		actionCommand.Keyboard.Key = Keyboard::KeyCode::W;
		actionCommand.Keyboard.State = Keyboard::KeyState::Down;
		Input::GetInstance().AddAxisCommand(actionCommand);
	}
}

void dae::PlayerController::Awake()
{
	
}

void dae::PlayerController::LateUpdate()
{
	
}

void dae::PlayerController::OnDeath()
{
	
}

void dae::PlayerController::Move(const glm::vec2& dir)
{
	const float width{ m_pCharacterController->GetCollider()->GetShape()->GetSize().x };

	GeometryUtils::Ray ray{};
	ray.direction = glm::vec3{ 0.f, 1.f, 0.f };
	ray.length = 4.f;
	ray.origin = GetTransform().GetWorldPosition() + glm::vec3{ width * 0.5f + dir.x, width, 0.f};

	CollisionHit hit{};
	const float epsilon{ 0.001f };
	bool moveVertical{ std::abs(dir.y) > epsilon };

	if (moveVertical)
	{
		glm::vec3 colliderPos{ m_pLadderCollider->GetTransform().GetLocalPosition() };
		colliderPos.y = width + dir.y * 2.f;
		m_pLadderCollider->GetTransform().SetLocalPosition(colliderPos);
	}

	if ((!moveVertical && GetScene()->GetPhysics().Raycast(ray, hit, PLAYER_COLLISION_LAYER)) ||
		(moveVertical && m_CanMoveVertical))
	{
		m_pCharacterController->Move(dir);
	}
	//else if ((moveVertical && m_CanMoveVertical))
	//{
	//	ray.direction = glm::vec3{ 0.f, dir.y, 0.f };
	//	ray.length = 1.f;
	//	//if (dir.y < 0.f)
	//	ray.origin.y += dir.y;
	//	if (!GetScene()->GetPhysics().Raycast(ray, hit, PLAYER_COLLISION_LAYER))
	//	{
	//		m_pCharacterController->Move(dir);
	//	}
	//}
}

void dae::PlayerController::OnOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == LADDER_COLLISION_LAYER)
	{
		m_CanMoveVertical = true;
	}
}

void dae::PlayerController::OnEndOverlap(const CollisionHit& hit)
{
	if (hit.pOtherCollider->GetCollisionLayer() == LADDER_COLLISION_LAYER)
	{
		m_CanMoveVertical = false;
	}
}
