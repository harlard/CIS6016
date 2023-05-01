#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class GlfwManager {
public:

    GlfwManager();
    ~GlfwManager();

    bool init(int widthW, int heightW);
    void clearWindow();
    void cleanup();


    GLFWwindow* getWindow() const;
    bool shouldClose() const;
    void swapBuffers() const;
    void pollEvents() const;
    float getDeltaTime();

private:
      GLFWwindow* window;
      int width;
      int height;
      double prevTime;
    };
