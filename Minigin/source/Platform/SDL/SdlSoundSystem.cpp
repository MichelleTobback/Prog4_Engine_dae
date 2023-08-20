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

	struct SdlMusicClip
	{
		~SdlMusicClip()
		{
			if (pMusic != nullptr)
				Mix_FreeMusic(pMusic);
		}
		Mix_Music* pMusic{ nullptr };
		bool isLoaded{ false };
	};

	enum class AudioType {Clip, Music};

	struct PlayAudioEvent
	{
		SoundId id{ INVALID_SOUND_ID };
		float volume{ 90.f };
		float duration{};
		int loops{};
		AudioType type{ AudioType::Clip };
	};

	class SdlSoundSystem::SdlSoundSystemImpl final
	{
	public:
		SdlSoundSystemImpl();
		~SdlSoundSystemImpl();

		SoundId AddSound(const std::string& filepath);
		void Play(SoundId id, float volume, int loop);
		void PlayForDuration(SoundId id, float volume, float duration);

		SoundId AddMusic(const std::string& filepath);
		void PlayMusic(SoundId id, float volume, int loop);
		void PlayMusicForDuration(SoundId music, float volume, float duration);
		void StopMusic();
		void PauseMusic();

		void SetMasterVolume(float volume);
		float GetMasterVolume() const;

		void Init();
		void Shutdown();

	private:
		float m_MasterVolume;
		std::vector<SdlAudioClip> m_Clips{};
		std::unordered_map<SoundId, std::string> m_IdsToPath{};
		std::vector<SdlMusicClip> m_MusicClips{};
		std::unordered_map<SoundId, std::string> m_MusicIdsToPath{};
		std::queue<PlayAudioEvent> m_EventQueue{};
		std::mutex m_Mutex{};
		std::condition_variable m_Condition{};
		bool m_Running{false};
		std::thread m_Worker{};

		void LoadClip(SoundId clip);
		void LoadMusic(SoundId music);
		void WorkerThreadFunc();
	};
}

dae::SdlSoundSystem::SdlSoundSystemImpl::SdlSoundSystemImpl()
{
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

	Init();

	m_MasterVolume = static_cast<float>(Mix_MasterVolume(-1));
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

void dae::SdlSoundSystem::SdlSoundSystemImpl::Play(SoundId id, float volume, int loop)
{
	if (id < m_Clips.size())
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		PlayAudioEvent audioEvent{};
		audioEvent.id = id;
		audioEvent.volume = volume;
		audioEvent.loops = loop;
		audioEvent.type = AudioType::Clip;
		m_EventQueue.push(audioEvent);
		m_Condition.notify_one();
	}
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::PlayForDuration(SoundId id, float volume, float duration)
{
	if (id < m_Clips.size())
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		PlayAudioEvent audioEvent{};
		audioEvent.id = id;
		audioEvent.volume = volume;
		audioEvent.loops = -1;
		audioEvent.duration = duration;
		audioEvent.type = AudioType::Clip;
		m_EventQueue.push(audioEvent);
		m_Condition.notify_one();
	}
}

dae::SoundId dae::SdlSoundSystem::SdlSoundSystemImpl::AddMusic(const std::string& filepath)
{
	for (auto& [soundId, path] : m_MusicIdsToPath)
	{
		if (path._Equal(filepath.c_str()))
			return soundId;
	}

	SoundId id{ static_cast<SoundId>(m_Clips.size()) };
	SdlMusicClip clip{};
	m_MusicClips.push_back(clip);
	m_MusicIdsToPath.emplace(id, filepath);
	return id;
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::PlayMusic(SoundId id, float volume, int loop)
{
	if (id < m_MusicClips.size())
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		PlayAudioEvent audioEvent{};
		audioEvent.id = id;
		audioEvent.volume = volume;
		audioEvent.loops = loop;
		audioEvent.type = AudioType::Music;
		m_EventQueue.push(audioEvent);
		m_Condition.notify_one();
	}
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::PlayMusicForDuration(SoundId music, float volume, float duration)
{
	if (music < m_MusicClips.size())
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		PlayAudioEvent audioEvent{};
		audioEvent.id = music;
		audioEvent.volume = volume;
		audioEvent.loops = -1;
		audioEvent.duration = duration;
		audioEvent.type = AudioType::Music;
		m_EventQueue.push(audioEvent);
		m_Condition.notify_one();
	}
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::StopMusic()
{
	Mix_RewindMusic();
	Mix_PauseMusic();
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::PauseMusic()
{ 
	Mix_PauseMusic();
}

void dae::SdlSoundSystem::SdlSoundSystemImpl::SetMasterVolume(float volume)
{
	m_MasterVolume = std::clamp(volume, 0.f, static_cast<float>(MIX_MAX_VOLUME));
	Mix_MasterVolume(static_cast<int>(volume));
	Mix_VolumeMusic(static_cast<int>(m_MasterVolume));
}

float dae::SdlSoundSystem::SdlSoundSystemImpl::GetMasterVolume() const
{
	return m_MasterVolume;
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

void dae::SdlSoundSystem::SdlSoundSystemImpl::LoadMusic(SoundId music)
{
	std::filesystem::path filePath{ ResourceManager::GetInstance().GetDataPath() + m_MusicIdsToPath[music] };

	auto& clip{ m_MusicClips[music] };
	clip.pMusic = Mix_LoadMUS(filePath.string().c_str());
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

			switch (playEvent.type)
			{
			case AudioType::Clip:
			{
				if (playEvent.id < m_Clips.size())
				{
					SdlAudioClip& clip{ m_Clips[playEvent.id] };
					if (!clip.isLoaded)
						LoadClip(playEvent.id);

					int channel
					{ (playEvent.duration < 0.01f)
						? Mix_PlayChannel(-1, clip.pChunk, playEvent.loops) 
						: Mix_PlayChannelTimed(-1, clip.pChunk, playEvent.loops, static_cast<int>(playEvent.duration * 1000))
					};
					if (channel != -1)
					{
						Mix_Volume(channel, static_cast<int>(playEvent.volume));
					}
				}
			}
			break;
			case AudioType::Music:
			{
				if (playEvent.id < m_MusicClips.size())
				{
					SdlMusicClip& clip{ m_MusicClips[playEvent.id] };
					if (!clip.isLoaded)
						LoadMusic(playEvent.id);

					int success{ Mix_PlayMusic(clip.pMusic, playEvent.loops) };
					if (success != -1)
					{
						Mix_VolumeMusic(static_cast<int>(m_MasterVolume));
					}
				}
			}
			break;
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

void dae::SdlSoundSystem::Play(SoundId id, float volume, int loop)
{
	m_pImpl->Play(id, volume, loop);
}

void dae::SdlSoundSystem::PlayForDuration(SoundId sound, float volume, float duration)
{
	m_pImpl->PlayForDuration(sound, volume, duration);
}

dae::SoundId dae::SdlSoundSystem::AddMusic(const std::string& filepath)
{
	return m_pImpl->AddMusic(filepath);
}

void dae::SdlSoundSystem::PlayMusic(SoundId id, float volume, int loop)
{
	m_pImpl->PlayMusic(id, volume, loop);
}

void dae::SdlSoundSystem::PlayMusicForDuration(SoundId sound, float volume, float duration)
{
	m_pImpl->PlayMusicForDuration(sound, volume, duration);
}

void dae::SdlSoundSystem::StopMusic()
{
	m_pImpl->StopMusic();
}

void dae::SdlSoundSystem::PauseMusic()
{
	m_pImpl->PauseMusic();
}

void dae::SdlSoundSystem::SetMasterVolume(float volume)
{
	m_pImpl->SetMasterVolume(volume);
}

float dae::SdlSoundSystem::GetMasterVolume() const
{
	return m_pImpl->GetMasterVolume();
}
