#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Sound {
    ALuint id;
    std::string filePath;
    glm::vec3 position;
    glm::vec3 velocity;
};

class OpenALManager {
public:
    OpenALManager();
    ~OpenALManager();

    // Sound loading and management methods
    void loadSound(const std::string& filePath, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f));
    void deleteSound(Sound& sound);
    void playSound(Sound& sound);
    void stopSound(Sound& sound);
    void setListenerPosition(const glm::vec3& position);
    void setListenerVelocity(const glm::vec3& velocity);
    void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);

private:
    ALCdevice* device;
    ALCcontext* context;

    // Sound buffer and source management methods
    ALuint loadSoundBuffer(const std::string& filePath);
    void deleteSoundBuffer(ALuint buffer);
    ALuint createSource();
    void deleteSource(ALuint source);

    std::vector<Sound> sounds;
};
