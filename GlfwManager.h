#pragma once

#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/glew.h>
#include <iostream>

class GlfwManager {
public:

    GlfwManager();
    ~GlfwManager();

    bool init(int widthW, int heightW);
    void cleanup();

    GLFWwindow* getWindow() const;
    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    float GetDeltaTime();

private:
      GLFWwindow* window;
      int width;
      int height;
      static double deltaTime;
    };
