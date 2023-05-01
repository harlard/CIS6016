#include "OpenALManager.h"
#include <iostream>
#include <stdexcept>
#include <sndfile.h>

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
    auto it = std::find_if(sounds.begin(), sounds.end(), [&](const Sound& s) { return &s == &sound; });
    if (it != sounds.end()) {
        deleteSource(it->id);
        sounds.erase(it);
    }
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

ALuint OpenALManager::loadSoundBuffer(const std::string& filePath)
{
    // Open the audio file using libsndfile
    SF_INFO file_info;
    SNDFILE* audio_file = sf_open(filePath.c_str(), SFM_READ, &file_info);
    if (!audio_file)
    {
        std::cerr << "Failed to open audio file: " << filePath << std::endl;
        return 0;
    }

    // Check that the format is supported by OpenAL
    if (file_info.channels > AL_FORMAT_STEREO16 || file_info.channels < AL_FORMAT_MONO8)
    {
        std::cerr << "Unsupported audio format for file: " << filePath << std::endl;
        return 0;
    }

    // Allocate a buffer for the audio data
    const ALsizei buffer_size = static_cast<ALsizei>(file_info.frames * file_info.channels);
    std::vector<char> audio_buffer(buffer_size);

    // Read the audio data into the buffer
    const sf_count_t frames_read = sf_read_raw(audio_file, audio_buffer.data(), buffer_size);
    if (frames_read != file_info.frames)
    {
        std::cerr << "Failed to read audio data from file: " << filePath << std::endl;
        return 0;
    }

    // Close the audio file
    sf_close(audio_file);

    // Generate a new buffer ID
    ALuint buffer_id = 0;
    alGenBuffers(1, &buffer_id);

    // Upload the audio data to the buffer
    if (file_info.channels == 1)
    {
        if (file_info.format == SF_FORMAT_PCM_U8)
        {
            alBufferData(buffer_id, AL_FORMAT_MONO8, audio_buffer.data(), buffer_size, file_info.samplerate);
        }
        else if (file_info.format == SF_FORMAT_PCM_16)
        {
            alBufferData(buffer_id, AL_FORMAT_MONO16, audio_buffer.data(), buffer_size, file_info.samplerate);
        }
    }
    else if (file_info.channels == 2)
    {
        if (file_info.format == SF_FORMAT_PCM_U8)
        {
            alBufferData(buffer_id, AL_FORMAT_STEREO8, audio_buffer.data(), buffer_size, file_info.samplerate);
        }
        else if (file_info.format == SF_FORMAT_PCM_16)
        {
            alBufferData(buffer_id, AL_FORMAT_STEREO16, audio_buffer.data(), buffer_size, file_info.samplerate);
        }
    }

    return buffer_id;
}


ALuint OpenALManager::createSource()
{
    ALuint source;
    alGenSources(1, &source);
    return source;
}

void OpenALManager::deleteSource(ALuint source)
{
    alDeleteSources(1, &source);
}
