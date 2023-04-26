#include "GameObjectManager.h"

GameObjectManager::GameObjectManager() {}

GameObjectManager::~GameObjectManager() {}

void GameObjectManager::update(float deltaTime) {
    for (auto gameObject : gameObjects) {
        gameObject->update(deltaTime);
    }
}

void GameObjectManager::draw()
{
    if (cameras.empty()) {
        return;
    }

    Camera& currentCamera = cameras[cameraInUse];
    glm::mat4 viewMatrix = currentCamera.getViewMatrix();
    glm::mat4 projectionMatrix = currentCamera.getProjectionMatrix();

    if(!skyboxes.empty()){
      Skybox& currentSkybox = skyboxes[skyBoxInUse];
      currentSkybox.draw(projectionMatrix, viewMatrix);
    }

    for (auto gameObject : gameObjects) {
        gameObject->draw(projectionMatrix, viewMatrix);
    }

    currentSkybox.draw(viewMatrix);
}

std::shared_ptr<GameObject> GameObjectManager::createGameObject(const std::string& meshFilePath) {
    auto gameObject = std::make_shared<GameObject>(meshFilePath);
    gameObjects.push_back(gameObject);
    return gameObject;
}

void GameObjectManager::destroyGameObject(std::shared_ptr<GameObject> gameObject) {
    auto gameObjectIt = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (gameObjectIt != gameObjects.end()) {
        gameObjects.erase(gameObjectIt);
    }
}

std::shared_ptr<GameObject> GameObjectManager::getGameObjectByName(const std::string& name) {
    for (auto gameObject : gameObjects) {
        if (gameObject->getName() == name) {
            return gameObject;
        }
    }
    return nullptr; // return null pointer if not found
}

void GameObjectManager::createNewCamera(std::string cameraId, const glm::vec3& position, const glm::vec3& up, const glm::vec3& front) {
    cameras.emplace_back(position, up, front);
    cameras.back().setCameraId(cameraId);
}

void GameObjectManager::useNextCamera() {
    cameraInUse = (cameraInUse + 1) % cameras.size();
}

void GameObjectManager::useCameraWithId(std::string cameraId) {
    for (size_t i = 0; i < cameras.size(); i++) {
        if (cameras[i].getCameraId() == cameraId) {
            cameraInUse = static_cast<int>(i);
            return;
        }
    }
    std::cout << "Camera with ID " << cameraId << " not found." << std::endl;
}

Camera* GameObjectManager::GetCurrentCamera() {
    if (cameras.empty()) {
        return nullptr;
    }
    return &cameras[cameraInUse];
}

void GameObjectManager::createNewSkyBox(std::string skyboxId, const std::string& directory) {
    skyboxes.emplace_back(directory);
    skyboxes.back().setSkyboxId(skyboxId);
}

void GameObjectManager::nextSkybox() {
    skyBoxInUse = (skyBoxInUse + 1) % skyboxes.size();
}

void GameObjectManager::useSkyboxWithId(std::string skyboxId) {
    for (size_t i = 0; i < skyboxes.size(); i++) {
        if (skyboxes[i].getSkyboxId() == skyboxId) {
            skyBoxInUse = static_cast<int>(i);
            return;
        }
    }
    std::cout << "Skybox with ID " << skyboxId << " not found." << std::endl;
}
