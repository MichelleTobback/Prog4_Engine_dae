#include "SdlSoundSystem.h"
#include "Managers/ResourceManager.h"

#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace dae
{
	struct SdlAudioClip
	{
		~SdlAudioClip()
		{
			if (pChunk != nullptr)
				Mix_FreeChunk(pChunk);
		}
		Mix_Chunk* pChunk{ nullptr };
		bool isLoaded{ false };
	};

	struct PlayAudioEvent
	{
		SoundId id{ INVALID_SOUND_ID };
		float volume{ 90.f };
	};

	class SdlSoundSystem::SdlSoundSystemImpl final
	{
	public:
		SdlSoundSystemImpl();
		~SdlSoundSystemImpl();

		SoundId AddSound(const std::string& filepath);
		void Play(SoundId id, float volume);

		void Init();
		void Shutdown();

	private:
		std::vector<SdlAudioClip> m_Clips{};
		std::unordered_map<SoundId, std::string> m_IdsToPath{};
		std::queue<PlayAudioEvent> m_EventQueue{};
		std::mutex m_Mutex{};
		std::condition_variable m_Condition{};
		bool m_Running{false};
		std::thread m_Worker{};

		void LoadClip(SoundId clip);
		void WorkerThreadFunc();
	};
}

dae::SdlSoundSystem::SdlSoundSystemImpl::SdlSoundSystemImpl()
{
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

	Init();
}

dae::SdlSoundSystem::SdlSoundSystemImpl::~SdlSoundSystemImpl()
{
	Shutdown();

	m_Clips.clear();
	Mix_CloseAudio();
	Mix_Quit();
}

dae::SoundId dae::SdlSoundSystem::SdlSoundSystemImpl::AddSound(const std::string& filepath)
{
	for (auto& [soundId, path] : m_IdsToPath)
	{
		if (path._Equal(filepath.c_str()))
			return soundId;
	}

	SoundId id{ static_cast<SoundId>(m_Clips.size()) };
	SdlAudioClip clip{};
	m_Clips.push_back(clip);
	m_IdsToPath.emplace(id, filepath);
	return id;
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::Play(SoundId id, float volume)
{
	if (id < m_Clips.size())
	{
		SdlAudioClip& clip{ m_Clips[id] };
		if (!clip.isLoaded)
			LoadClip(id);
		 
		std::unique_lock<std::mutex> lock(m_Mutex);
		PlayAudioEvent audioEvent{};
		audioEvent.id = id;
		audioEvent.volume = volume;
		m_EventQueue.push(audioEvent);
		m_Condition.notify_one();
	}
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::Init()
{
	m_Running = true;
	m_Worker = std::thread(&SdlSoundSystemImpl::WorkerThreadFunc, this);
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::Shutdown()
{
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Running = false;
		m_Condition.notify_one();
	}

	if (m_Worker.joinable())
	{
		m_Worker.join();
	}
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::LoadClip(SoundId id)
{
	std::filesystem::path filePath{ ResourceManager::GetInstance().GetDataPath() + m_IdsToPath[id] };

	auto& clip{ m_Clips[id] };
	clip.pChunk = Mix_LoadWAV(filePath.string().c_str());
	clip.isLoaded = true;
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::WorkerThreadFunc()
{
	while (m_Running)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this]() 
			{ 
				return !m_EventQueue.empty() || !m_Running; 
			});

		if (!m_EventQueue.empty())
		{
			PlayAudioEvent playEvent = m_EventQueue.front();
			m_EventQueue.pop();
			lock.unlock();

			if (playEvent.id < m_Clips.size())
			{
				SdlAudioClip& clip{ m_Clips[playEvent.id] };
				int channel{ Mix_PlayChannel(-1, clip.pChunk, 0) };
				if (channel != -1)
				{
					Mix_Volume(channel, static_cast<int>(playEvent.volume));
				}
			}
		}
	}
}

dae::SdlSoundSystem::SdlSoundSystem()
{
	m_pImpl = std::make_unique<SdlSoundSystemImpl>();
}

dae::SdlSoundSystem::~SdlSoundSystem()
{
}

dae::SoundId dae::SdlSoundSystem::AddSound(const std::string& filepath)
{
	return m_pImpl->AddSound(filepath);
}

void dae::SdlSoundSystem::Play(SoundId id, float volume)
{
	m_pImpl->Play(id, volume);
}
