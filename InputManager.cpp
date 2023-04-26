#include "InputManager.h"

InputManager::InputManager(GLFWwindow* window) : window(window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->handleKeyInput(key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->handleMouseInput(button, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        inputManager->handleCursorPos(xpos, ypos);
    });
}

InputManager::~InputManager() {}

void InputManager::processInput() {
    // Check for input events
    glfwPollEvents();
}

void InputManager::handleKeyInput(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void InputManager::handleMouseInput(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // Do something with the cursor position and left mouse click event
    }
}

void InputManager::handleCursorPos(double xpos, double ypos) {
    // Do something with the cursor position
}
