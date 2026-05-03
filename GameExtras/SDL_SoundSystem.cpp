
#include "SDL_SoundSystem.hpp"
#include <SDL3_mixer/SDL_mixer.h>
#include <stdexcept>
#include <map>
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
};

dae::SDL_SoundSystem::SDL_SoundSystem(std::vector<std::pair<sound_id, std::string>> sounds)
	:m_pImpl{std::make_unique<Impl>(sounds)}
{
    m_SoundThread = std::jthread([this](std::stop_token token) {
        while (!token.stop_requested()) {
            m_Queue.DoAllSounds();
        }
        });
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
}

void dae::SDL_SoundSystem::Play(sound_id const id, float const volume)
{
    m_Queue.AddToQueue([this, id, volume]() {
        m_pImpl->Play(id, volume);
        });
}

void dae::SDL_SoundSystem::Destroy()
{
    m_SoundThread.request_stop();
    m_Queue.Quit();
    //i realise that doing this manually with a jthread is weird but i need this one to be destroyed before pimpl is
    //i also cant do it in the destructor since this class is managed by a Singleton so it gets deleted AFTER SDL_Quit is called
    //which has to happen after the mix_quit is called
    m_SoundThread.join();
    m_pImpl->Destroy();
}

dae::Impl::Impl(std::vector<std::pair<sound_id, std::string>> sounds)
{
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
}

void dae::Impl::Destroy()
{
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
