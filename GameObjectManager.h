#pragma once

#include <vector>
#include <memory>
#include "Skybox.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <iostream>
#include "GameObject.h"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();

    void update(float deltaTime);
    void draw();

    std::shared_ptr<GameObject> createGameObject(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void addGameObject(std::shared_ptr<GameObject> gameObject);
    void destroyGameObject(std::shared_ptr<GameObject> gameObject);
    std::shared_ptr<GameObject> getGameObjectByName(const std::string& name);

    void createNewCamera(const std::string cameraId, const glm::vec3& position, const glm::vec3& up, const glm::vec3& front);
    Camera* getCurrentCamera();
    Camera* getCamera(std::string cameraId);
    void useNextCamera();
    void useCamera(std::string cameraId);
    void removeCamera(std::string cameraId);


    void createNewSkybox(const std::string skyboxId, const std::string& directory);
    void useNextSkybox();
    Skybox* getCurrentSkybox();
    Skybox* getSkybox(std::string skyboxId);
    void useSkybox(std::string skyboxId);
    void removeSkybox(std::string skyboxId);

    void createLight(const std::string lightId, const std::string type, glm::vec3 color, float intensity);
    void deleteLight(std::string lightId);
    Light* getLight(std::string lightId);

    void cleanup();

private:
    std::string cameraInUse;
    std::string skyboxInUse;
    std::vector<Light> lights;
    std::vector<Camera> cameras;
    std::vector<Skybox> skyboxes;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    };
