#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Mesh.h"

class GameObject {
public:
    GameObject(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    virtual ~GameObject();

    virtual void updateUniforms();

    void setShader(ShaderProgram* shaderProgram);
    ShaderProgram* getShader();

    void setMesh(Mesh* mesh);
    Mesh* getMesh() const;

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

    virtual void update(float deltaTime);
    void updateChildren(float deltaTime);
    void draw(const glm::mat4& parentTransform, const glm::mat4& view);
    void drawChilden(const glm::mat4& parentTransform, const glm::mat4& view);

    std::string getName() const;
    void setName(const std::string& name);


    void addChild(GameObject* child);
    void removeChild(GameObject* child);

protected:

    ShaderProgram* shaderProgram;
    std::string name;
    Mesh* mesh;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::vector<GameObject*> children;
};
