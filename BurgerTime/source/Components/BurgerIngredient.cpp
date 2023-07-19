#include "BurgerIngredient.h"
#include "Scene/Scene.h"
#include "Component/RenderComponent.h"
#include "Component/SpriteAtlasComponent.h"
#include "Audio/AudioClip.h"

std::unique_ptr<dae::AudioClip> dae::BurgerIngredient::m_pOverlapSound{ nullptr };
std::unique_ptr<dae::AudioClip> dae::BurgerIngredient::m_pFallSound{nullptr};

dae::BurgerIngredient::BurgerIngredient(GameObject* pOwner, IngredientType type, SpriteAtlasComponent* pSpriteAtlas, RigidBody2DComponent* pRigidBody)
	: Component(pOwner), m_pRigidBody{pRigidBody}, m_Type{type}
{
	glm::vec4 spriteSrc { 112.f, 48.f + m_TileSize * static_cast<int>(type), m_TileSize, m_TileSize};

	if (!m_pOverlapSound)
		m_pOverlapSound = std::make_unique<AudioClip>("Sounds/13_item1.wav");
	if (!m_pFallSound)
		m_pFallSound = std::make_unique<AudioClip>("Sounds/28_jingle.wav");

	for (int i{}; i < m_Length; ++i)
		m_AllFlags |= (1 << (i + 1));

	for (int i{}; i < m_Length; ++i)
	{
		GameObject* pTileCollider{ GetScene()->Instantiate(0u, GetOwner(), glm::vec3{ m_TileSize * i + m_TileSize * 0.25f, 0.f, 0.f }) };
		GameObject* pTile{ GetScene()->Instantiate(0u, pTileCollider, glm::vec3{-m_TileSize * 0.25f, 0.f, 0.f})};
		uint32_t spriteIdx{ pSpriteAtlas->AddSprite(spriteSrc) };
		spriteSrc.x += m_TileSize;
		pTile->AddComponent<SpriteRenderComponent>(pSpriteAtlas->GetSprite(spriteIdx))->SetLayer(1);
	
		BoxCollider2DComponent* pCollider{ pTileCollider->AddComponent<BoxCollider2DComponent>() };
		pCollider->SetCollisionLayer(INGREDIENT_COLLISION_LAYER);
		pCollider->SetShape(pTileCollider->AddComponent<QuadComponent>(glm::vec2{ m_TileSize * 0.5f, m_TileSize }));
		pCollider->SetTrigger(true);
		pRigidBody->AddCollider(pCollider);

		pRigidBody->GetOnBeginOverlap() += [this, pCollider, i](const CollisionHit& hit)
		{
			int tileFlag{1 << (i + 1)};
			const bool isCurrentCollider{ hit.pCollider == pCollider };
			const bool alreadyTriggered{ static_cast<bool>(m_WalkedOnTilesFlags & tileFlag) };
			if (isCurrentCollider && !alreadyTriggered && !m_OnPlate &&
				hit.pOtherCollider->GetCollisionLayer() == dae::CollisionLayer::Two) //player
			{
				const glm::vec3 pos{ pCollider->GetOwner()->GetTransform().GetLocalPosition() };
				pCollider->GetOwner()->GetTransform().SetLocalPosition(glm::vec3{ pos.x, 2.f, pos.z });
				m_WalkedOnTilesFlags |= tileFlag;

				if (m_WalkedOnTilesFlags == m_AllFlags)
				{
					Fall();
				}
				else
					m_pOverlapSound->Play();
			}
			else if (m_WalkedOnTilesFlags == m_AllFlags
				&& hit.pOtherCollider->GetCollisionLayer() != CollisionLayer::Two 
				&& hit.pOtherCollider->GetCollisionLayer() != CollisionLayer::Three
				&& GetTransform().GetWorldPosition().y > m_CurrentYPos) 
			{
				if (hit.pOtherCollider->GetCollisionLayer() == INGREDIENT_COLLISION_LAYER && !m_OnPlate
					&& GetTransform().GetWorldPosition().y < hit.pOtherCollider->GetTransform().GetWorldPosition().y)
				{
					BurgerIngredient* pOtherIngredient{ BurgerIngredient::GetFromCollider(hit.pOtherCollider)};
					pOtherIngredient->Fall();
				}
				else if (m_WalkedOnTilesFlags == m_AllFlags 
					&& hit.pOtherCollider->GetCollisionLayer() != CollisionLayer::Five)
				{
					Reset();
					m_pRigidBody->SetVelociy(glm::vec3{ 0.f, 0.f, 0.f });
					CacheCurrentYPos();
				}
			}
		};
	}
}

void dae::BurgerIngredient::Fall()
{
	const float fallSpeed{ 45.f };
	m_pRigidBody->SetVelociy(glm::vec3{ 0.f, fallSpeed, 0.f });
	m_pFallSound->Play();

	if (m_WalkedOnTilesFlags != m_AllFlags)
		SetAllTilesTriggered();
}

dae::BurgerIngredient* dae::BurgerIngredient::GetFromCollider(ColliderComponent* pCollider)
{
	GameObject* pGameObject{ (pCollider->GetOwner()->IsRoot()) 
		? pCollider->GetOwner() 
		: pCollider->GetOwner()->GetParent() };

	return pGameObject->GetComponent<BurgerIngredient>();
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
	CacheCurrentYPos();
}

void dae::BurgerIngredient::PlateEnter()
{
	m_OnPlate = true;
}

void dae::BurgerIngredient::CacheCurrentYPos()
{
	m_CurrentYPos = m_pRigidBody->GetCollider(0)->GetTransform().GetWorldPosition().y + m_TileSize * 2.f;
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
