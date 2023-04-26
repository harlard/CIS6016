#include "Skybox.h"

Skybox::Skybox(const std::string& id, const std::string& directory) : skyboxId_(id)
{
    faces_ = {
        directory + "right.jpg",
        directory + "left.jpg",
        directory + "top.jpg",
        directory + "bottom.jpg",
        directory + "front.jpg",
        directory + "back.jpg"
    };
    loadCubemap();
    loadShaders();
    setup();
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &cubemapcubemapTextureID_);
    glDeleteVertexArrays(1, &vaoID_);
    glDeleteBuffers(1, &vboID_);
    glDeleteProgram(shaderProgramID_);
}

void Skybox::setup()
{
    glGenVertexArrays(1, &vaoID_);
    glGenBuffers(1, &vboID_);

    glBindVertexArray(vaoID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), &vertices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Skybox::loadCubemap()
{
    glGenTextures(1, &cubemapTextureID_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID_);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces_.size(); i++)
    {
        unsigned char* data = stbi_load(faces_[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces_[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::loadShaders()
{
    shaderProgramId_ = std::make_shared<ShaderProgram>();
    shaderProgramId_->create();
    shaderProgramId_->attachShader("shaders/skybox.vert", GL_VERTEX_SHADER);
    shaderProgramId_->attachShader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
    shaderProgramId_->link();
}

void Skybox::draw(const glm::mat4& projection, const glm::mat4& view)
{
    glDepthMask(GL_FALSE);
    shaderProgramId_->use();
    shaderProgramId_->setMat4("view", view);
    shaderProgramId_->setMat4("projection", projection);
    glBindVertexArray(vaoID_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}
