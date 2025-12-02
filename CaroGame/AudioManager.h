#pragma once
#include <string>
#include <map>
#include <AL/al.h>
#include <AL/alc.h>

enum class SoundEffect {
    Move,
    Win,
    Draw
};

class AudioManager {
public:
    static AudioManager& getInstance();
    void init();
    void cleanup();
    void playSound(SoundEffect);
    void playBackgroundMusic(const std::string& path);
    void stopBackgroundMusic();
    void toggleMute();
    void increaseVolume();
    void decreaseVolume();
    bool isMuted() const;
    int getVolumeLevel() const;

private:
    AudioManager(); 
    ~AudioManager() = default;

    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

    void updateAllSourceVolumes();

    ALuint loadWav(const std::string& path);

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::map<SoundEffect, ALuint> soundBuffers;
    std::map<SoundEffect, ALuint> soundSources;
    ALuint bgSource = 0;
    ALuint bgBuffer = 0;

    bool m_isMuted;
    float m_globalVolume;
};