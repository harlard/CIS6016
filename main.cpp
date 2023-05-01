#include "GlfwManager.h"
#include "OpenALManager.h"
//#include "ODEManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "Skybox.h"
#include "Camera.h"
#include "Ovni.h"
#include "ShaderProgram.h"

int main() {
    // Initialize the managers
    GlfwManager glfwManager;
    OpenALManager openAlManager;
  //  ODEManager odeManager;
    GameObjectManager gameObjectManager;

    //initialize delta time
    float deltaTime;

    // Initialize the window and input callbacks
    glfwManager.init(800, 600);
    InputManager inputManager(glfwManager.getWindow());

    //Set the Camera
    glm::vec3 cameraPosition(10.0f, 10.0f, 3.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    gameObjectManager.createNewCamera("main_camera", cameraPosition, cameraUp, cameraFront);
    gameObjectManager.useCamera("main_camera");
    gameObjectManager.getCamera("main_camera")->setViewport(800,600);
    gameObjectManager.getCamera("main_camera")->setFieldofView(glm::radians(45.0f));
    gameObjectManager.getCamera("main_camera")->setNearClip(0.1f);
    gameObjectManager.getCamera("main_camera")->setFarClip(500.0f);
    //create skybox
    ShaderProgram skyboxShader = ShaderProgram("Shaders/SkyboxShader.vertexshader", "Shaders/SkyboxShader.fragmentshader");
    gameObjectManager.createNewSkybox("test", "Assets/Skybox/Space/blue");
    gameObjectManager.getSkybox("test")->setShaderProgram(std::make_shared<ShaderProgram>(skyboxShader));
    gameObjectManager.getSkybox("test")->setLightColor(glm::vec3(1.0f,1.0f,1.0f));
    gameObjectManager.getSkybox("test")->setLightIntensity(1.0f);
    gameObjectManager.useSkybox("test");

    //create a source of light
  //  ShaderProgram lightShader = ShaderProgram("Shaders/PointLight.vertexshader", "Shaders/PointLight.fragmentshader");
    gameObjectManager.createLight("pointLight1", "Point", glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    gameObjectManager.getLight("pointLight1")->setColor(glm::vec3(1.0f, 1.0f, 10.0f));
    gameObjectManager.getLight("pointLight1")->setIntensity(100.0f);
  //  gameObjectManager.getLight("pointLight1")->setShader(&lightShader);

    // Add the gameObject
    // create a new GameObject
    std::shared_ptr<Ovni> myGameObject = std::make_shared<Ovni>("Ovni", glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f));

    // add it to the GameObjectManager
    gameObjectManager.addGameObject(myGameObject);


    // Game loop
    while (!glfwManager.shouldClose()) {

        gameObjectManager.getCamera("main_camera")->lookAt(gameObjectManager.getGameObjectByName("Ovni")->getPosition());

        glfwManager.clearWindow();
        deltaTime = glfwManager.getDeltaTime();
        // Process input
        inputManager.processInput();

        // Updates
    //    odeManager.update(deltaTime);
        gameObjectManager.update(deltaTime);

        // Render the game objects
        gameObjectManager.draw();

        // Swap buffers and poll events
        glfwManager.swapBuffers();
        glfwManager.pollEvents();
    }

    // Clean up
  //  odeManager.cleanup();

    gameObjectManager.cleanup();
    glfwManager.cleanup();

    return 0;
    }
