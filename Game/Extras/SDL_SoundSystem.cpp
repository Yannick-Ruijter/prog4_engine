
#include "SDL_SoundSystem.hpp"
#include <SDL3_mixer/SDL_mixer.h>
#include <stdexcept>
#include <map>
#include <iostream>
using namespace dae;

class dae::Impl {
public:
	Impl(std::vector<std::pair<sound_id, std::string>> sounds);
	~Impl();
	void Play(sound_id id, float volume);
    void Destroy();
private:
	std::map<sound_id, std::pair<MIX_Audio*, std::vector<MIX_Track*>>> m_Sounds{};
	MIX_Mixer* m_Mixer{};

    SoundEventQueue m_Queue{};
    mutable std::mutex m_Mutex{};
    std::unique_ptr<Impl> m_pImpl;
    std::thread m_SoundThread;
    std::atomic<bool> m_StopThread{ false };
};

dae::SDL_SoundSystem::SDL_SoundSystem(std::vector<std::pair<sound_id, std::string>> sounds)
	:m_pImpl{std::make_unique<Impl>(sounds)}
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
    if (!m_HasBeenDestroyed) Destroy();
}

void dae::SDL_SoundSystem::Play(sound_id const id, float const volume)
{
    m_pImpl->Play(id, volume);
    
}

void dae::SDL_SoundSystem::Destroy()
{
    m_pImpl->Destroy();
    m_HasBeenDestroyed = true;
}

dae::Impl::Impl(std::vector<std::pair<sound_id, std::string>> sounds)
{

    //using threads instead of jthreads because of emscriptem not supporting them yet
    m_SoundThread = std::thread([this]() {
        m_Mutex.lock();
        while (!m_StopThread) {
            m_Queue.DoAllSounds();
        }
        m_Mutex.unlock();
        });

    MIX_Init();
    m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!m_Mixer)
    {
        SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
        return;
    }

    for (auto const& [id, path] : sounds)
    {
        MIX_Audio* clip = MIX_LoadAudio(m_Mixer, path.c_str(), true);
        if (!clip)
            SDL_Log("Failed to load audio '%s': %s", path.c_str(), SDL_GetError());
        m_Sounds.emplace(id, std::pair{ clip, std::vector<MIX_Track*>{} });
    }
}

dae::Impl::~Impl()
{
}

void dae::Impl::Play(sound_id id, float volume)
{
    m_Queue.AddToQueue([this, id, volume]() {
        auto it{ m_Sounds.find(id) };
        if (it == m_Sounds.end()) return;

        MIX_Audio* audio = it->second.first;
        auto& tracks = it->second.second;

        // find a free track
        MIX_Track* track = nullptr;
        for (MIX_Track* t : tracks)
        {
            if (!MIX_TrackPlaying(t))
            {
                track = t;
                break;
            }
        }

        //if we have no free track rn
        if (!track)
        {
            track = MIX_CreateTrack(m_Mixer);
            tracks.push_back(track);
        }

        MIX_SetTrackAudio(track, audio);
        MIX_SetTrackGain(track, volume);
        MIX_PlayTrack(track, 0);
        });
}

void dae::Impl::Destroy()
{
    m_StopThread = true;
    //i realise i could use a condition variable but this only takes 1 member to do
    m_Mutex.lock();
    m_Mutex.unlock();
    m_Queue.Quit();
    m_SoundThread.join();

    if (!m_Mixer) return;

    for (auto& [id, entry] : m_Sounds)
    {
        for (MIX_Track* track : entry.second)
        {
            MIX_StopTrack(track, 0);
            MIX_DestroyTrack(track);
        }
        MIX_DestroyAudio(entry.first);
    }

    MIX_DestroyMixer(m_Mixer);
    MIX_Quit();
}
