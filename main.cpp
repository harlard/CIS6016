#include "GlfwManager.h"
#include "OpenAlManager.h"
#include "ODEManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "Skybox.h"
#include "Camera.h"

int main() {
    // Initialize the managers
    GlewManager glewManager;
    glfwManager glfwManager;
    OpenAlManager openAlManager;
    ODEManager odeManager;
    InputManager inputManager;
    GameObjectManager objectManager;

    //initialize delta time
    float deltaTime;

    // Initialize the window and input callbacks
    glfwManager.init(800, 600);
    inputManager.init(glfwManager.getWindow());
    odeManager.init();

    //Set the Camera
    glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    gameObjectManager.createNewCamera("main_camera", cameraPosition, cameraUp, cameraFront);


    // Game loop
    while (!glfwManager.shouldClose()) {

        deltaTime = glfwManager.getDeltaTime();
        // Process input
        inputManager.processInput();

        // Updates
        objectManager.update(deltaTime);
        odeManager.update(deltaTime);

        // Render the game objects
        objectManager.draw();

        // Swap buffers and poll events
        glfwManager.swapBuffers();
        glfwManager.pollEvents();
    }

    // Clean up
    odeManager.cleanup();
    openAlManager.cleanup();
    glfwManager.cleanup();
    glewManager.cleanup();

    return 0;
}
