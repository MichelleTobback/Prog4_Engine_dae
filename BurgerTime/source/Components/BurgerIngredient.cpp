#include "BurgerIngredient.h"
#include "Scene/Scene.h"
#include "Component/RenderComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Audio/AudioClip.h"

#include "State/GameState/GameState.h"
#include "States/GameModes/BTGameMode.h"

#include "States/Ingredients/IngredientFallState.h"
#include "States/Ingredients/IngredientIdleState.h"
#include "States/Ingredients/IngredientInPlateState.h"
#include "State/StateMachine.h"

#include "BurgerTime.h"

std::unique_ptr<dae::AudioClip> dae::BurgerIngredient::m_pOverlapSound{ nullptr };
std::unique_ptr<dae::AudioClip> dae::BurgerIngredient::m_pFallSound{nullptr};

dae::BurgerIngredient::BurgerIngredient(GameObject* pOwner, IngredientType type, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody, uint32_t reward)
	: Component(pOwner), m_pRigidBody{pRigidBody}, m_Type{type}, m_Reward{reward}
{
	glm::vec4 spriteSrc { 112.f, 48.f + m_TileSize * static_cast<int>(type), m_TileSize, m_TileSize};

	if (!m_pOverlapSound)
		m_pOverlapSound = std::make_unique<AudioClip>("Sounds/13_item1.wav");
	if (!m_pFallSound)
		m_pFallSound = std::make_unique<AudioClip>("Sounds/28_jingle.wav");

	for (int i{}; i < m_Length; ++i)
		m_AllFlags |= (1 << (i + 1));

	GameObject* pColliderObj{ GetScene()->Instantiate(0u, GetOwner())};
	QuadComponent* pQuad{ pColliderObj->AddComponent<QuadComponent>(glm::vec2{ m_TileSize * 4.f, m_TileSize }) };
	BoxCollider2DComponent* pCollider{ pColliderObj->AddComponent<BoxCollider2DComponent>() };
	pCollider->SetCollisionIgnoreLayer(BurgerTime::PLAYER_COLLISION_LAYER, true);
	pCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
	pCollider->SetCollisionLayer(BurgerTime::INGREDIENT_COLLISION_LAYER);
	pCollider->SetShape(pQuad);
	pCollider->SetTrigger(false);
	pRigidBody->AddCollider(pCollider);

	for (int i{}; i < m_Length; ++i)
	{
		GameObject* pTileColliderObj{ GetScene()->Instantiate(0u, GetOwner(), glm::vec3{ m_TileSize * i + m_TileSize * 0.25f, 0.f, 0.f }) };
		GameObject* pTile{ GetScene()->Instantiate(0u, pTileColliderObj, glm::vec3{-m_TileSize * 0.25f, 0.f, 0.f})};
		uint32_t spriteIdx{ pSpriteAtlas->AddSprite(spriteSrc) };
		spriteSrc.x += m_TileSize;
		pTile->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spriteIdx))->SetLayer(1);
	
		BoxCollider2DComponent*  pTileCollider = pTileColliderObj->AddComponent<BoxCollider2DComponent>();
		pTileCollider->SetCollisionLayer(BurgerTime::INGREDIENT_COLLISION_LAYER);
		pTileCollider->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
		pTileCollider->SetCollisionIgnoreLayer(BurgerTime::LEVEL_COLLISION_LAYER, true);
		pTileCollider->SetShape(pTileColliderObj->AddComponent<QuadComponent>(glm::vec2{ m_TileSize * 0.5f, m_TileSize }));
		pTileCollider->SetTrigger(true);
		pRigidBody->AddCollider(pTileCollider);
	}

	pRigidBody->GetOnBeginOverlap() += std::bind(&BurgerIngredient::OnBeginOverlap, this, std::placeholders::_1);
	pRigidBody->GetOnEndOverlap() += std::bind(&BurgerIngredient::OnEndOverlap, this, std::placeholders::_1);

	m_pStates.pInPlate = std::make_unique<IngredientInPlateState>(this);
	m_pStates.pIdle = std::make_unique<IngredientIdleState>(this);
	m_pStates.pFall = std::make_unique<IngredientFallState>(this);
	m_pStateMachine = GetOwner()->AddComponent<StateMachine>(m_pStates.pIdle.get());
}

void dae::BurgerIngredient::Fall()
{
	const float epsilon{ 0.001f };
	if (m_pRigidBody->GetVelocity().y < epsilon)
	{
		m_pCurrentGameMode->GetScore() += m_Reward;
		m_pRigidBody->GetCollider(0)->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, false);

		const float fallSpeed{ 60.f };
		m_pRigidBody->SetVelociy(glm::vec3{ 0.f, fallSpeed, 0.f });
		m_pFallSound->Play();

		if (m_WalkedOnTilesFlags != m_AllFlags)
			SetAllTilesTriggered();
	}
}

dae::BurgerIngredient* dae::BurgerIngredient::GetFromCollider(ColliderComponent* pCollider)
{
	GameObject* pGameObject{ (pCollider->GetOwner()->IsRoot()) 
		? pCollider->GetOwner() 
		: pCollider->GetOwner()->GetParent() };

	return pGameObject->GetComponent<BurgerIngredient>();
}

void dae::BurgerIngredient::AddReward()
{
	++m_Reward;
}

int dae::BurgerIngredient::GetWalkedFlags() const
{
	return m_WalkedOnTilesFlags;
}

void dae::BurgerIngredient::SetWalkedFlags(int flags)
{
	m_WalkedOnTilesFlags = flags;
}

void dae::BurgerIngredient::Awake()
{
	m_pCurrentGameMode = dynamic_cast<BTGameMode*>(&GameState::GetInstance().GetGameMode());
	m_pStateMachine->SetState(m_pStates.pIdle.get());
	m_OnPlate = false;
}

void dae::BurgerIngredient::PlateEnter()
{
	m_OnPlate = true;
	m_pCurrentGameMode->AddIngredient();
}

void dae::BurgerIngredient::OnBeginOverlap(const CollisionHit& hit)
{
	IngredientState* pState{ dynamic_cast<IngredientState*>(&m_pStateMachine->GetCurrent()) };
	if (pState)
		pState->OnBeginOverlap(hit);

	//if (m_OnPlate)
	//	return;
	//
	//int i{};
	//const auto& colliders{ m_pRigidBody->GetColliders() };
	//const int numColliders{ static_cast<int>(colliders.size()) };
	//for (int index{ 1 }; index <= numColliders; ++index)
	//{
	//	const bool isValidCollider{ index < numColliders };
	//	if (isValidCollider && hit.pCollider == colliders[index])
	//	{
	//		i = index;
	//		break;
	//	}
	//	else if (!isValidCollider)
	//		return;
	//}
	//
	//int tileFlag{ 1 << i };
	//const bool alreadyTriggered{ static_cast<bool>(m_WalkedOnTilesFlags & tileFlag) };
	//const bool isFirstTrigger{ i == 1 };
	//const CollisionLayer collidingLayer{ hit.pOtherCollider->GetCollisionLayer() };
	//TransformComponent& transform{ hit.pCollider->GetOwner()->GetTransform() };
	//const glm::vec3 pos{ transform.GetLocalPosition() };
	//ColliderComponent* pCollider{ m_pRigidBody->GetCollider(0) };
	//
	//if (!alreadyTriggered && collidingLayer == BurgerTime::PLAYER_COLLISION_LAYER)
	//{
	//	transform.SetLocalPosition(glm::vec3{ pos.x, 2.f, pos.z });
	//	m_WalkedOnTilesFlags |= tileFlag;
	//
	//	if (m_WalkedOnTilesFlags == m_AllFlags)
	//	{
	//		pCollider->SetTrigger(true);
	//		Fall();
	//	}
	//	else
	//		m_pOverlapSound->Play();
	//}
	//else if (isFirstTrigger && collidingLayer == BurgerTime::INGREDIENT_COLLISION_LAYER
	//	&& pos.y < hit.pOther->GetTransform().GetWorldPosition().y)
	//{
	//	hit.pOtherCollider->SetTrigger(true);
	//	BurgerIngredient::GetFromCollider(hit.pOtherCollider)->Fall();
	//
	//	Reset();
	//}
}

void dae::BurgerIngredient::OnEndOverlap(const CollisionHit& hit)
{
	IngredientState* pState{ dynamic_cast<IngredientState*>(&m_pStateMachine->GetCurrent()) };
	if (pState)
		pState->OnEndOverlap(hit);
}

void dae::BurgerIngredient::Reset()
{
	const auto& pChildren{ GetOwner()->GetChildren() };
	for (auto& pChild : pChildren)
	{
		const glm::vec3 pos{ pChild->GetTransform().GetLocalPosition() };
		pChild->GetTransform().SetLocalPosition(glm::vec3{ pos.x, 0.f, pos.z });
	}
	m_WalkedOnTilesFlags = 0;

	m_pRigidBody->GetCollider(0)->SetCollisionIgnoreLayer(BurgerTime::ENEMY_COLLISION_LAYER, true);
}

void dae::BurgerIngredient::SetAllTilesTriggered()
{
	m_WalkedOnTilesFlags = m_AllFlags;
	const auto& pChildren{ GetOwner()->GetChildren() };
	for (auto& pChild : pChildren)
	{
		const glm::vec3 pos{ pChild->GetTransform().GetLocalPosition() };
		pChild->GetTransform().SetLocalPosition(glm::vec3{ pos.x, 2.f, pos.z });
	}
}
