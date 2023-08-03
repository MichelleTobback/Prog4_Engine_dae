#pragma once
#include "Component/Component.h"

#include "Component/RenderComponent.h"
#include "SpriteComponent.h"
#include <memory>

namespace dae
{
	struct SpriteAnimFrame
	{
		SpriteComponent* pSprite{ nullptr };
		float duration{ 0.12f };
	};
	struct SpriteAnimClip
	{
		std::vector<SpriteAnimFrame> m_Frames{};
		float speed{1.f};
	};

	class SpriteAnimatorComponent final : public Component
	{
	public:
		SpriteAnimatorComponent(GameObject* pOwner);
		virtual ~SpriteAnimatorComponent() = default;

		SpriteAnimatorComponent(const SpriteAnimatorComponent& other) = delete;
		SpriteAnimatorComponent(SpriteAnimatorComponent&& other) = delete;
		SpriteAnimatorComponent& operator=(const SpriteAnimatorComponent& other) = delete;
		SpriteAnimatorComponent& operator=(SpriteAnimatorComponent&& other) = delete;

		virtual void Update() override;

		void SetRendererComponent(SpriteRenderComponent* pRenderer);
		size_t AddAnimClip(const std::shared_ptr<SpriteAnimClip>& pAnimClip);
		size_t AddAnimClip(const std::vector<SpriteAnimFrame>& frames, float speed = 1.f);
		void PlayClip(size_t animClipIdx);
		void PlayClip(const std::shared_ptr<SpriteAnimClip>& pAnimClip);
		void Play();
		void PlayFromStart();
		void Pause();

		void SetPlayBackspeed(float speed);
		float GetPlaybackSpeed() const;
		const SpriteAnimClip& GetCurrentClip() const;

	private:
		SpriteRenderComponent* m_pRenderer{ nullptr };
		std::vector<std::shared_ptr<SpriteAnimClip>> m_pAnimClips;
		float m_PlaybackSpeed{ 1.f };
		SpriteAnimClip* m_pCurrentClip{ nullptr };
		size_t m_CurrentFrame{};
		float m_CurrentTime{};
		bool m_Running{ false };
	};
}