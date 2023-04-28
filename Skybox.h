#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "stb_image.h"
#include "ShaderProgram.h"

class Skybox {
public:
    Skybox(const std::string& skyboxId, const std::string& directory);
    ~Skybox();

    std::string getSkyboxId() const;

    void draw(const glm::mat4& view, const glm::mat4& projection);

    void setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);

private:
    std::string skyboxId_;
    GLuint vaoID_, vboID_;
    GLuint cubemapTextureID_;
    std::vector<std::string> faces_;

    std::shared_ptr<ShaderProgram> shaderProgram_;

    static const float vertices_[108];

    void setup();
    void loadCubemap();
};
