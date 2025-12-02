#include "AudioManager.h"
#include <iostream>
#include <vector>
#include<limits>
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h" 
#ifndef ALC_HRTF_SOFT
#define ALC_HRTF_SOFT 0x1992
#endif
ALuint AudioManager::loadWav(const std::string& path) {
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalFrames;
    short* pDecodedData = drwav_open_file_and_read_pcm_frames_s16(
        path.c_str(),
        &channels,
        &sampleRate,
        &totalFrames,
        nullptr
    );

    if (pDecodedData == NULL) {
        std::cerr << "Failed to load WAV (dr_wav): " << path << "\n";
        return 0;
    }
    if (totalFrames == 0) {
        std::cerr << "WAV file is empty (0 frames): " << path << "\n";
        drwav_free(pDecodedData, nullptr); 
        return 0;
    }

    drwav_uint64 dataSizeInBytes = totalFrames * channels * sizeof(short);
    if (dataSizeInBytes > std::numeric_limits<ALsizei>::max()) {
        std::cerr << "WAV file is too large to load into OpenAL buffer: " << path << "\n";
        drwav_free(pDecodedData, nullptr);
        return 0;
    }

    ALenum format;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    }
    else {
        format = AL_FORMAT_STEREO16;
    }

    ALsizei dataSize = (ALsizei)(totalFrames * channels * sizeof(short));

    ALuint buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, pDecodedData, dataSize, sampleRate);
    drwav_free(pDecodedData, nullptr);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "OpenAL Error after loading " << path << ": " << error << "\n";
        if (buffer && alIsBuffer(buffer)) {
            alDeleteBuffers(1, &buffer);
        }
        return 0;
    }

    return buffer;
}

AudioManager::AudioManager() :
    device(nullptr),
    context(nullptr),
    bgSource(0),
    bgBuffer(0),
    m_isMuted(false),
    m_globalVolume(1.0f) {
}

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::init() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open OpenAL device\n";
        return;
    }
    const ALCint context_attribs[] = {
        ALC_HRTF_SOFT, ALC_FALSE,
        0 
    };
    context = alcCreateContext(device, context_attribs);
    if (!context) {
        std::cerr << "Failed to create OpenAL context\n";
        alcCloseDevice(device);
        return;
    }
    alcMakeContextCurrent(context);

    soundBuffers[SoundEffect::Move] = loadWav("audio/move_effect.wav");
    soundBuffers[SoundEffect::Win] = loadWav("audio/win_effect.wav");
    soundBuffers[SoundEffect::Draw] = loadWav("audio/draw_effect.wav");

    for (auto& [effect, buffer] : soundBuffers) {
        if (buffer == 0) continue;
        ALuint src;
        alGenSources(1, &src);
        alSourcei(src, AL_BUFFER, buffer);
        alSourcef(src, AL_GAIN, m_globalVolume);
        soundSources[effect] = src;
    }

    std::cout << "OpenAL initialized.\n";
}

void AudioManager::cleanup() {
    for (auto& [effect, src] : soundSources)
        alSourceStop(src);
    if (bgSource)
        alSourceStop(bgSource);
    for (auto& [effect, src] : soundSources)
        alDeleteSources(1, &src);
    if (bgSource)
        alDeleteSources(1, &bgSource);

    // Xóa buffers
    for (auto& [effect, buf] : soundBuffers)
        alDeleteBuffers(1, &buf);
    if (bgBuffer)
        alDeleteBuffers(1, &bgBuffer);

    alcMakeContextCurrent(nullptr);
    if (context) alcDestroyContext(context);
    if (device) alcCloseDevice(device);
}

void AudioManager::playSound(SoundEffect effect) {
    if (m_isMuted) return;
    if (soundSources.count(effect)) {
        alSourcef(soundSources[effect], AL_GAIN, m_globalVolume);
        alSourcePlay(soundSources[effect]);
    }
}

void AudioManager::playBackgroundMusic(const std::string& path) {
    if (bgSource) {
        alSourceStop(bgSource);
        alSourcei(bgSource, AL_BUFFER, 0); 
    }
    if (bgBuffer) {
        alDeleteBuffers(1, &bgBuffer);
        bgBuffer = 0;
    }

    bgBuffer = loadWav(path);
    if (!bgBuffer) return; 

    if (!bgSource) alGenSources(1, &bgSource); 

    alSourcei(bgSource, AL_BUFFER, bgBuffer);
    alSourcei(bgSource, AL_LOOPING, AL_TRUE);

    float currentVolume = m_isMuted ? 0.0f : m_globalVolume;
    alSourcef(bgSource, AL_GAIN, currentVolume);
    alSourcePlay(bgSource);
}

void AudioManager::stopBackgroundMusic() {
    if (bgSource) alSourceStop(bgSource);
}

void AudioManager::updateAllSourceVolumes() {
    float newVolume = m_isMuted ? 0.0f : m_globalVolume;

    if (bgSource) {
        alSourcef(bgSource, AL_GAIN, newVolume);
    }
    for (auto& [effect, src] : soundSources) {
        alSourcef(src, AL_GAIN, newVolume);
    }
}

void AudioManager::toggleMute() {
    m_isMuted = !m_isMuted;
    updateAllSourceVolumes();
}

void AudioManager::increaseVolume() {
    m_globalVolume += 0.1f;
    if (m_globalVolume > 1.0f) m_globalVolume = 1.0f;
    updateAllSourceVolumes();
}

void AudioManager::decreaseVolume() {
    m_globalVolume -= 0.1f;
    if (m_globalVolume < 0.0f) m_globalVolume = 0.0f;
    updateAllSourceVolumes();
}

bool AudioManager::isMuted() const {
    return m_isMuted;
}

int AudioManager::getVolumeLevel() const {
    return static_cast<int>(m_globalVolume * 100);
}