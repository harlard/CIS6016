#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Skybox {
public:
    Skybox(cconst std::string& directory);
    ~Skybox();

    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    std::string skyboxId_
    GLuint vaoID_, vboID_;
    GLuint cubemapTextureID_;
    GLuint shaderProgramID_;

    std::vector<std::string> faces_;

    void setup();
    void loadCubemap();
    void loadShaders();
};
