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
    void useNextCamera();
    void useCameraWithId(std::string cameraId);
    Camera* GetCurrentCamera();

    void createNewSkyBox(std::string skyboxId, const std::string& directory);
    void nextSkybox();
    void useSkyboxWithId(std::string skyboxId);



private:
    int cameraInUse;
    int skyBoxInUse;
    std::vector<Camera> cameras;
    std::vector<Skybox> skyboxes;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
};
