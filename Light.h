#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "ShaderProgram.h"

class Light {
public:
    Light(const std::string lightId, const std::string type, glm::vec3 color, float intensity);

    virtual void updateUniforms() const;

    void setType(std::string type);
    std::string getType() const;

    void setColor(glm::vec3 color);
    glm::vec3 getColor() const;

    void setIntensity(float intensity);
    float getIntensity() const;

    void setPosition(glm::vec3 position);
    glm::vec3 getPosition() const;

    void setDirection(glm::vec3 direction);
    glm::vec3 getDirection() const;

    void setRotation(glm::vec3 rotation);
    glm::vec3 getRotatio() const;

    void setCutoff(float cutoff);
    float getCutoff() const;

    void setOuterCutoff(float outerCutoff);
    float getOuterCutoff() const;

    void setAreaSize(glm::vec2 areaSize);
    glm::vec2 getAreaSize() const;

    std::string getLightId() const;

    void setShader(ShaderProgram* shaderProgram);
    ShaderProgram* getShader();
    void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
    ShaderProgram* m_shaderProgram;
    std::string m_type;
    glm::vec3 m_color;
    float m_intensity;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_rotation;
    float m_cutoff;
    float m_outerCutoff;
    glm::vec2 m_areaSize;
    std::string m_lightId;
};
