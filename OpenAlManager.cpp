#include "OpenALManager.h"
#include <iostream>
#include <stdexcept>

OpenALManager::OpenALManager() {
    device = alcOpenDevice(NULL);
    if (!device) {
        throw std::runtime_error("Failed to open OpenAL device");
    }

    context = alcCreateContext(device, NULL);
    if (!context) {
        throw std::runtime_error("Failed to create OpenAL context");
    }

    if (!alcMakeContextCurrent(context)) {
        throw std::runtime_error("Failed to make OpenAL context current");
    }

}

OpenALManager::~OpenALManager() {
    for (auto sound : sounds) {
        deleteSound(sound);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void OpenALManager::loadSound(const std::string& filePath, const glm::vec3& position, const glm::vec3& velocity) {
    Sound sound;
    sound.filePath = filePath;
    sound.position = position;
    sound.velocity = velocity;

    ALuint buffer = loadSoundBuffer(filePath);
    ALuint source = createSource();

    alSourcei(source, AL_BUFFER, buffer);
    alSource3f(source, AL_POSITION, position.x, position.y, position.z);
    alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);

    sound.id = source;
    sounds.push_back(sound);
}

void OpenALManager::deleteSound(Sound& sound) {
    deleteSource(sound.id);
    sounds.erase(std::remove(sounds.begin(), sounds.end(), sound), sounds.end());
}

void OpenALManager::playSound(Sound& sound) {
    alSourcePlay(sound.id);
}

void OpenALManager::stopSound(Sound& sound) {
    alSourceStop(sound.id);
}

void OpenALManager::setListenerPosition(const glm::vec3& position) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void OpenALManager::setListenerVelocity(const glm::vec3& velocity) {
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void OpenALManager::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
    float orientation[6] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, orientation);
}

ALuint OpenALManager::loadSoundBuffer(const std::string& filePath) {
    ALuint buffer;

    alGenBuffers(1, &buffer);

    ALenum format;
    ALsizei size;
    ALvoid* data;

    // Load sound file using stb_vorbis library
    int channels;
    int sampleRate;
    short* samples;

    samples = stb_vorbis_decode_filename(filePath.c_str(), &channels, &sampleRate);
    if (!samples) {
        std::cerr << "Error loading sound file " << filePath << std::endl;
        return 0;
    }

    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    }
    else {
        format = AL_FORMAT_STEREO16;
    }

    size = sampleRate * channels * sizeof(short);

    data = static_cast<void*>(samples);

    alBufferData(buffer, format, data, size, sampleRate);

    return buffer;
}

void OpenALManager::deleteSoundBuffer(ALuint buffer) {
    alDeleteBuffers(1, &buffer);
}

ALuint OpenALManager::createSource() {
    ALuint source;
    alGenSources(1, &source);
    return source;
}

void OpenALManager::deleteSource(ALuint source) {
    alDeleteSources(1, &source);
}
