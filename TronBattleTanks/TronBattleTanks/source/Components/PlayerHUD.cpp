#include "PlayerHUD.h"
#include "Scene/GameObject.h"

#include "Components/HealthComponent.h"
#include "Component/TextComponent.h"
#include "Prefabs.h"
#include "Component/TransformComponent.h"
#include "PlayerScore.h"

#include <string>

void dae::PlayerHUD::Init()
{
	m_pPlayer1Health = m_pPlayer1->GetComponent<HealthComponent>();
	m_pPlayer2Health = m_pPlayer2->GetComponent<HealthComponent>();
	m_pPlayer1Score = m_pPlayer1->GetComponent<PlayerScore>();
	m_pPlayer2Score = m_pPlayer2->GetComponent<PlayerScore>();

	const std::string levensPrefix{ "Levens : " };
	const std::string scorePrefix{ "Score : " };

	const glm::vec4 color1{ 0.f, 1.f, 0.f, 1.f };
	const glm::vec4 color2{ 0.5f, 0.f, 0.9f, 1.f };

	//Player 1 score Text
	{
		auto pText{ Prefab::CreateTextObject(
			SceneManager::GetInstance().GetCurrent(),
			scorePrefix + std::to_string(m_pPlayer1Score->GetScore()),
			color1) };
		pText->GetOwner()->AttachToGameObject(GetOwner());
		TransformComponent& tc{ pText->GetOwner()->GetTransform() };
		tc.SetLocalPosition(glm::vec3{ 0.f, 150.f, 0.f });
		m_pSetPlayer1Score = std::make_unique<PlayerScoreTextBinding>(pText, scorePrefix);
		m_pPlayer1Score->GetOnScoreChanged()->AddObserver(m_pSetPlayer1Score.get());
	}
	//Player 1 levens Text
	{
		auto pText{ Prefab::CreateTextObject(
			SceneManager::GetInstance().GetCurrent(), 
			levensPrefix + std::to_string(m_pPlayer1Health->GetValue()),
			color1)};
		pText->GetOwner()->AttachToGameObject(GetOwner());
		TransformComponent& tc{ pText->GetOwner()->GetTransform() };
		tc.SetLocalPosition(glm::vec3{ 0.f, 170.f, 0.f });
		m_pSetPlayer1Health = std::make_unique<PlayerHealthTextBinding>(pText, levensPrefix);
		m_pPlayer1Health->GetOnHealthChanged()->AddObserver(m_pSetPlayer1Health.get());
	}

	//Player 2 score Text
	{
		auto pText{ Prefab::CreateTextObject(
			SceneManager::GetInstance().GetCurrent(),
			scorePrefix + std::to_string(m_pPlayer2Score->GetScore()),
			color2) };
		pText->GetOwner()->AttachToGameObject(GetOwner());
		TransformComponent& tc{ pText->GetOwner()->GetTransform() };
		tc.SetLocalPosition(glm::vec3{ 0.f, 250.f, 0.f });
		m_pSetPlayer2Score = std::make_unique<PlayerScoreTextBinding>(pText, scorePrefix);
		m_pPlayer2Score->GetOnScoreChanged()->AddObserver(m_pSetPlayer2Score.get());
	}
	//Player 2 levens Text
	{
		auto pText{ Prefab::CreateTextObject(
			SceneManager::GetInstance().GetCurrent(),
			levensPrefix + std::to_string(m_pPlayer2Health->GetValue()),
			color2) };
		pText->GetOwner()->AttachToGameObject(GetOwner());
		TransformComponent& tc{ pText->GetOwner()->GetTransform() };
		tc.SetLocalPosition(glm::vec3{ 0.f, 270.f, 0.f });
		m_pSetPlayer2Health = std::make_unique<PlayerHealthTextBinding>(pText, levensPrefix);
		m_pPlayer2Health->GetOnHealthChanged()->AddObserver(m_pSetPlayer2Health.get());
	}
}

void dae::PlayerHUD::Update()
{
	
}

dae::PlayerHealthTextBinding::PlayerHealthTextBinding(TextComponent* pText, const std::string& healthPrefix)
	: m_pText{ pText }
	, m_playerHealthPrefix{ healthPrefix }
{
}

void dae::PlayerHealthTextBinding::Invoke(const Event& event, Subject*)
{
	if (event.dataSize == 0)
		return;

	auto value{ reinterpret_cast<float*>(event.data) };
	auto text{ m_playerHealthPrefix + std::to_string(*value) };
	m_pText->SetText(text);
}

dae::PlayerScoreTextBinding::PlayerScoreTextBinding(TextComponent* pText, const std::string& healthPrefix)
	: m_pText{ pText }
	, m_playerHealthPrefix{ healthPrefix }
{
}

void dae::PlayerScoreTextBinding::Invoke(const Event& event, Subject*)
{
	if (event.dataSize == 0)
		return;

	auto value{ reinterpret_cast<int*>(event.data) };
	auto text{ m_playerHealthPrefix + std::to_string(*value) };
	m_pText->SetText(text);
}
