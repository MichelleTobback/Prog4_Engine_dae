#include "SpriteAnimatorComponent.h"
#include "Core/Time.h"

dae::SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::SpriteAnimatorComponent::Update()
{
	if (m_Running && m_pCurrentClip)
	{
		float deltaTime{ Time::GetInstance().GetDeltaTime() };

		m_CurrentTime += (deltaTime * m_PlaybackSpeed * m_pCurrentClip->speed);
		SpriteAnimFrame& currentFrame{ m_pCurrentClip->m_Frames[m_CurrentFrame] };
		bool frameChanged{ false };
		if (m_CurrentTime >= currentFrame.duration)
		{
			++m_CurrentFrame;
			m_CurrentTime = 0.f;
			frameChanged = true;
		}
		if (m_CurrentFrame >= m_pCurrentClip->m_Frames.size())
		{
			m_CurrentFrame = 0;
			frameChanged = true;
		}
		if (frameChanged)
		{
			m_pRenderer->SetSpriteComponent(currentFrame.pSprite);
		}
	}
}

void dae::SpriteAnimatorComponent::SetRendererComponent(SpriteRenderComponent* pRenderer)
{
	m_pRenderer = pRenderer;
}

size_t dae::SpriteAnimatorComponent::AddAnimClip(const std::shared_ptr<SpriteAnimClip>& pAnimClip)
{
	m_pAnimClips.push_back(pAnimClip);
	if (!m_pCurrentClip)
	{
		m_pCurrentClip = pAnimClip.get();
		m_pRenderer->SetSpriteComponent(m_pCurrentClip->m_Frames[m_CurrentFrame].pSprite);
	}
	return m_pAnimClips.size() - 1;
}

size_t dae::SpriteAnimatorComponent::AddAnimClip(const std::vector<SpriteAnimFrame>& frames, float speed)
{
	return AddAnimClip(std::make_shared<SpriteAnimClip>(frames, speed));
}

void dae::SpriteAnimatorComponent::PlayClip(size_t animClipIdx)
{
	if (m_pAnimClips.size() > animClipIdx)
	{
		PlayClip(m_pAnimClips[animClipIdx]);
	}
}

void dae::SpriteAnimatorComponent::PlayClip(const std::shared_ptr<SpriteAnimClip>& pAnimClip)
{
	m_pCurrentClip = pAnimClip.get();
	Play();
}

void dae::SpriteAnimatorComponent::Play()
{
	m_Running = true;
}

void dae::SpriteAnimatorComponent::PlayFromStart()
{
	m_CurrentTime = 0.f;
	m_CurrentFrame = 0;
	Play();
}

void dae::SpriteAnimatorComponent::Pause()
{
	m_Running = false;
}

void dae::SpriteAnimatorComponent::SetPlayBackspeed(float speed)
{
	m_PlaybackSpeed = speed;
}

float dae::SpriteAnimatorComponent::GetPlaybackSpeed() const
{
	return m_PlaybackSpeed;
}

const dae::SpriteAnimClip& dae::SpriteAnimatorComponent::GetCurrentClip() const
{
	return *m_pCurrentClip;
}
