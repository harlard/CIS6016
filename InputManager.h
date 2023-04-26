#pragma once

#include <GLFW/glfw3.h>

class InputManager {
public:
    InputManager(GLFWwindow* window);
    ~InputManager();
    void processInput();

    // Define methods for handling specific input events
    void handleKeyInput(int key, int scancode, int action, int mods);
    void handleMouseInput(int button, int action, int mods);
    void handleCursorPos(double xpos, double ypos);

private:
    GLFWwindow* window;
};
