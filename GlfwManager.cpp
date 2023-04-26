#include "GlfwManager.h"

GlfwManager::GlfwManager() : window(nullptr) {}

GlfwManager::~GlfwManager() {
    cleanup();
}

bool GlfwManager::init(int widthW, int heightW) {
  width = widthW;
  height = heightW;

  // Initialize GLFW
  if (!glfwInit()) {
      std::cerr << "Error: Could not initialize GLFW!" << std::endl;
      exit(-1);
  }

  // Create a window and OpenGL context
  window = glfwCreateWindow(width, height, "My Game", nullptr, nullptr);
  if (!window) {
      glfwTerminate();
      std::cerr << "Error: Could not create GLFW window!" << std::endl;
      exit(-1);
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
      std::cerr << "Error: Could not initialize GLEW!" << std::endl;
      exit(-1);
  }

  // Set the viewport size
  glfwGetFramebufferSize(window, width, height);
  glViewport(0, 0, width, height);
  
  // Initialize glad
  if (!gladLoadGL()) {
      // Handle error
      return -1;
  }

  return(true);
}

void GlfwManager::cleanup() {

    gladLoaderUnload();
    glewShutdown();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

GLFWwindow* GlfwManager::getWindow() const {
    return window;
}

bool GlfwManager::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void GlfwManager::swapBuffers() const {
    glfwSwapBuffers(window);
}

void GlfwManager::pollEvents() const {
    glfwPollEvents();
}


float GlfwManager::getDeltaTime(){
// Calculate deltaTime
double currentTime = glfwGetTime();
float deltaTime = static_cast<float>(currentTime - prevTime);
prevTime = currentTime;
return deltaTime;
}