#include "SpriteAnimatorComponent.h"
#include "Core/Time.h"

//=================================================================
//	SpriteAnimatorComponent
//=================================================================

dae::SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

dae::SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* pOwner, SpriteRenderComponent* pRenderer)
	: Component(pOwner), m_pRenderer{ pRenderer }
{
}


void dae::SpriteAnimatorComponent::Update()
{
	if (m_Running && m_pCurrentClip && m_pCurrentClip->frames.size() > 0)
	{
		float deltaTime{ Time::GetInstance().GetDeltaTime() };

		m_CurrentTime += (deltaTime * m_PlaybackSpeed * m_pCurrentClip->speed);
		SpriteAnimFrame& currentFrame{ m_pCurrentClip->frames[m_CurrentFrame] };
		bool frameChanged{ false };
		if (m_CurrentTime >= currentFrame.duration)
		{
			++m_CurrentFrame;
			m_CurrentTime = 0.f;
			frameChanged = true;
		}
		if (m_CurrentFrame >= m_pCurrentClip->frames.size())
		{
			m_CurrentFrame = 0;
			frameChanged = true;
			m_pCurrentClip->pOnClipEndDelegate->Invoke();
		}
		if (frameChanged)
		{
			m_pRenderer->SetSpriteComponent(currentFrame.pSprite);

			for (auto& animEvent : m_pCurrentClip->events)
			{
				if (animEvent.first == m_CurrentFrame)
					animEvent.second.Invoke();
			}
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
		m_pRenderer->SetSpriteComponent(m_pCurrentClip->frames[m_CurrentFrame].pSprite);
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
	if (m_CurrentFrame >= m_pCurrentClip->frames.size())
		m_CurrentFrame = 0;
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

dae::SpriteAnimClip& dae::SpriteAnimatorComponent::GetClip(size_t id)
{
	return *m_pAnimClips[id];
}

void dae::SpriteAnimatorComponent::SetClip(size_t id)
{
	assert(id < m_pAnimClips.size() && "no valid id");
	m_pCurrentClip = m_pAnimClips[id].get();
}

//=================================================================
//	SpriteAnimClip
//=================================================================

dae::SpriteAnimClip::SpriteAnimClip()
	: pOnClipEndDelegate{ std::make_unique<Delegate<void()>>() }
{
}

dae::SpriteAnimClip::SpriteAnimClip(const std::vector<SpriteAnimFrame>& _frames, float _speed)
	: frames{ _frames }, speed{_speed}, pOnClipEndDelegate{ std::make_unique<Delegate<void()>>() }
{
}

dae::SpriteAnimationEvent& dae::SpriteAnimClip::AddAnimEvent(size_t frame)
{
	events.push_back(std::make_pair(frame, Delegate<void()>{}));
	return events.back();
}

dae::Delegate<void()>& dae::SpriteAnimClip::GetAnimEvent(size_t frame)
{
	for (auto& animEvent : events)
	{
		if (animEvent.first == frame)
			return animEvent.second;
	}
	return AddAnimEvent(frame).second;
}
