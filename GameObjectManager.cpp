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

    Camera& currentCamera = getCurrentCamera();
    if(currentCamera == nullptr){
        return;
    }

    glm::mat4 viewMatrix = currentCamera.getViewMatrix();
    glm::mat4 projectionMatrix = currentCamera.getProjectionMatrix();

    if(!skyboxes.empty()){
      Skybox& currentSkybox = getCurrentSkybox();
      if(currentSkybox != nullptr){
        currentSkybox.draw(projectionMatrix, viewMatrix);
      }
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
    Camera camera(position, up, front);
    camera.setCameraId(cameraId);
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
    return getCamera(cameraInUse);

}

void GameObjectManager::useCamera(std::string cameraId) {
    Camera* camera = getCamera(cameraId);
    if (camera != nullptr) {
        cameraInUse = camera.getCameraId());
    }
}

void GameObjectManager::createNewSkybox(const std::string& skyboxId, const std::string& directory)
{
    Skybox newSkybox(directory);
    newSkybox.setSkyboxId(skyboxId);
    skyboxes.push_back(newSkybox);
}

void GameObjectManager::nextSkybox() {
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
    Skybox* skybox = getSkybox(skyboxId);
    if (skybox != nullptr) {
        skyBoxInUse = skybox.getSkyboxId();
    }
}

void GameObjectManager::cleanup() {
  for (auto gameObject : gameObjects) {
      delete gameObject;
  }
  gameObjects.clear();

  for (auto camera : cameras) {
      delete camera;
  }
  cameras.clear();

  for (auto skybox : skyboxes) {
      delete skybox;
  }
  skyboxes.clear();
}
