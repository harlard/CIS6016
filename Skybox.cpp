#include "Skybox.h"
#include <iostream>
#include <ostream>
#include <memory>

const float Skybox::vertices_[108] = {
    // Back face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    // Front face
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // Left face
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // Right face
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    // Bottom face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    // Top face
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f
};

Skybox::Skybox(const std::string& skyboxId, const std::string& directory)
: skyboxId_(skyboxId), shaderProgram_(nullptr)
{
    // Define the faces of the cubemap
    faces_ = {
    directory + "/right.png",
    directory + "/left.png",
    directory + "/top.png",
    directory + "/bottom.png",
    directory + "/front.png",
    directory + "/back.png"
    };

    setup();
    // Load the cubemap texture
    loadCubemap();
  }

Skybox::~Skybox()
{
// Delete the VAO and VBO
  glDeleteVertexArrays(1, &vaoID_);
  glDeleteBuffers(1, &vboID_);
  // Delete the cubemap texture
  glDeleteTextures(1, &cubemapTextureID_);

}

std::string Skybox::getSkyboxId() const
{
    return skyboxId_;
}

void Skybox::setLightColor(const glm::vec3& lightColor) {
    lightColor_ = lightColor;
}

void Skybox::setLightIntensity(float lightIntensity) {
    lightIntensity_ = lightIntensity;
}

void Skybox::updateUniforms()
{
  shaderProgram_->setVec3("envLightColor", lightColor_);
  shaderProgram_->setFloat("envLightIntensity", lightIntensity_);
}


void Skybox::draw(const glm::mat4& view, const glm::mat4& projection)
{
    // Disable depth testing to make sure the skybox is always drawn behind everything else
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // Use the skybox shader program
    if (shaderProgram_){
        shaderProgram_->use();
        //updateUniforms();
        shaderProgram_->setMat4("view", view);
        shaderProgram_->setMat4("projection", projection);
      }

    // Set the view and projection matrices in the shader program

    // Bind the cubemap texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID_);

    // Draw the skybox
    glBindVertexArray(vaoID_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glActiveTexture(GL_TEXTURE0);
  //  glBindVertexArray(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    //GLint skyboxLoc = shaderProgram_->getUniformLocation("skybox");
  //  glUniform1i(skyboxLoc, 0);

    // Re-enable depth testing
    glDepthMask(GL_TRUE);


    if (shaderProgram_)
        shaderProgram_->unuse();
}

void Skybox::setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram)
{
  shaderProgram_ = shaderProgram;
}

void Skybox::setup()
{
    // Create and bind the VAO
    glGenVertexArrays(1, &vaoID_);
    glBindVertexArray(vaoID_);
    // Create and bind the VBO
    glGenBuffers(1, &vboID_);
    glBindBuffer(GL_ARRAY_BUFFER, vboID_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Skybox::loadCubemap() - Failed to load texture: " << faces_[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
