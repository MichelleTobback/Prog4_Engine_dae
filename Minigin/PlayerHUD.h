#pragma once
#include "Component.h"
#include "Observer.h"


namespace dae
{
	class TextComponent;
	class PlayerHealthTextBinding final : public Observer
	{
	public:
		PlayerHealthTextBinding(TextComponent* pText, const std::string& healthPrefix = "");
		virtual ~PlayerHealthTextBinding() = default;

		virtual void Invoke(const Event& event) override;

	private:
		TextComponent* m_pText;
		const std::string m_playerHealthPrefix;
	};
	class PlayerScoreTextBinding final : public Observer
	{
	public:
		PlayerScoreTextBinding(TextComponent* pText, const std::string& healthPrefix = "");
		virtual ~PlayerScoreTextBinding() = default;

		virtual void Invoke(const Event& event) override;

	private:
		TextComponent* m_pText;
		const std::string m_playerHealthPrefix;
	};

	class PlayerScore;
	class HealthComponent;
	class TextureRendererComponent;
	class PlayerHUD final : public Component
	{
	public:
		PlayerHUD(GameObject* pOwner, GameObject* pPlayer1, GameObject* pPlayer2)
			: Component{pOwner}
			, m_pPlayer1{ pPlayer1 }, m_pPlayer2{ pPlayer2 }
			, m_pPlayer1Health{ nullptr }, m_pPlayer2Health{nullptr}
		{ Init(); }
		virtual ~PlayerHUD() override = default;

		PlayerHUD(const PlayerHUD& other) = delete;
		PlayerHUD(PlayerHUD&& other) = delete;
		PlayerHUD& operator=(const PlayerHUD& other) = delete;
		PlayerHUD& operator=(PlayerHUD&& other) = delete;

		void Init();
		virtual void Update() override;

	private:
		GameObject* m_pPlayer1;
		GameObject* m_pPlayer2;
		HealthComponent* m_pPlayer1Health;
		HealthComponent* m_pPlayer2Health;
		PlayerScore* m_pPlayer1Score;
		PlayerScore* m_pPlayer2Score;


		std::unique_ptr<PlayerHealthTextBinding> m_pSetPlayer1Health{ nullptr };
		std::unique_ptr<PlayerHealthTextBinding> m_pSetPlayer2Health{ nullptr };

		std::unique_ptr<PlayerScoreTextBinding> m_pSetPlayer1Score{ nullptr };
		std::unique_ptr<PlayerScoreTextBinding> m_pSetPlayer2Score{ nullptr };
	};
}