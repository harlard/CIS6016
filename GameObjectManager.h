#pragma once

#include <vector>
#include <memory>
#include "Skybox.h"
#include "Camera.h"
#include <string>
#include <iostream>
#include "GameObject.h"

class GameObjectManager {
public:
    GameObjectManager();
    ~GameObjectManager();

    void update(float deltaTime);
    void draw();

    std::shared_ptr<GameObject> createGameObject(const std::string& meshFilePath);
    void destroyGameObject(std::shared_ptr<GameObject> gameObject);
    std::shared_ptr<GameObject> getGameObjectByName(const std::string& name) {

    void createNewCamera(std::string cameraId, const glm::vec3& position, const glm::vec3& up, const glm::vec3& front);
    Camera* getCurrentCamera();
    Camera* getCamera(std::string cameraId);
    void useNextCamera();
    void useCamera(std::string cameraId);
    void removeCamera(std::string cameraId);


    void createNewSkyBox(std::string skyboxId, const std::string& directory);
    void useNextSkybox();
    Skybox* getCurrentSkybox();
    Skybox* getCamera(std::string skyboxId);
    void useSkybox(std::string skyboxId);
    void removeSkybox(std::string skyboxId);



    void cleanup();

private:
    std::string cameraInUse;
    std::string skyBoxInUse;
    std::vector<Camera> cameras;
    std::vector<Skybox> skyboxes;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
};
