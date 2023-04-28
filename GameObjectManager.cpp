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

    // Get the current camera and check if it's valid
    Camera* currentCameraPtr = getCurrentCamera();
    if (currentCameraPtr == nullptr) {
        return;
    }
    Camera& currentCamera = *currentCameraPtr;

    glm::mat4 viewMatrix = currentCamera.getViewMatrix();
    glm::mat4 projectionMatrix = currentCamera.getProjectionMatrix();

    Skybox* currentSkyboxPtr = getCurrentSkybox();
    if (currentSkyboxPtr != nullptr) {
        Skybox& currentSkybox = *currentSkyboxPtr;
        currentSkybox.draw(projectionMatrix, viewMatrix);
    }


    for (auto gameObject : gameObjects) {
        gameObject->draw(projectionMatrix, viewMatrix);
    }

}

std::shared_ptr<GameObject> GameObjectManager::createGameObject(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
    auto gameObject = std::make_shared<GameObject>(name, position, rotation, scale);
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
    Camera camera(cameraId, position, up, front);
    cameras.push_back(camera);
}

void GameObjectManager::useNextCamera() {
    if (cameras.empty()) {
        return;
    }

    auto it = std::find_if(cameras.begin(), cameras.end(),
        [this](const Camera& cam) { return cam.getCameraId() == cameraInUse; });

    if (it == cameras.end()) {
        cameraInUse = cameras[0].getCameraId();
        return;
    }

    it++;
    if (it == cameras.end()) {
        it = cameras.begin();
    }

    cameraInUse = it->getCameraId();
}

void GameObjectManager::useCamera(std::string cameraId) {
  Camera* cameraPtr = getCamera(cameraId);
  if (cameraPtr != nullptr) {
      Camera& camera = *cameraPtr;
      cameraInUse = camera.getCameraId();
  }
}

Camera* GameObjectManager::getCurrentCamera() {
    if (cameras.empty()) {
        return nullptr;
    }
    return getCamera(cameraInUse);

}


void GameObjectManager::createNewSkybox(const std::string skyboxId, const std::string& directory)
{
    Skybox newSkybox(skyboxId, directory);
    skyboxes.push_back(newSkybox);
}

void GameObjectManager::useNextSkybox() {
    if (skyboxes.empty()) {
        return;
    }

    auto it = std::find_if(skyboxes.begin(), skyboxes.end(),
        [this](const Skybox& sky) { return sky.getSkyboxId() == skyboxInUse; });

    if (it == skyboxes.end()) {
        skyboxInUse = skyboxes[0].getSkyboxId();
        return;
    }

    it++;
    if (it == skyboxes.end()) {
        it = skyboxes.begin();
    }

    skyboxInUse = it->getSkyboxId();
}

void GameObjectManager::useSkybox(std::string skyboxId) {
    Skybox* skyboxPtr = getSkybox(skyboxId);
    if (skyboxPtr != nullptr) {
        Skybox& skybox = *skyboxPtr;
        skyboxInUse = skybox.getSkyboxId();
    }
}

void GameObjectManager::cleanup() {
  for (auto gameObject : gameObjects) {
      delete gameObject.get();
  }
  gameObjects.clear();

  cameras.clear();

  skyboxes.clear();
}
