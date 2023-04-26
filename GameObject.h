#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.h"

class GameObject {
public:
    GameObject(const std::string& name, Mesh* mesh, unsigned int shaderProgram const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
    virtual ~GameObject();

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

    void update(float deltaTime);
    void draw(const glm::mat4& parentTransform, const glm::mat4& view);

    std::string getName() const;
    void setName(const std::string& name);

    void setMesh(const std::shared_ptr<Mesh>& mesh);
    Mesh* getMesh() const;

    void addChild(GameObject* child);
    void removeChild(GameObject* child);

private:

    unsigned int shaderProgram;
    std::string name;
    Mesh* mesh;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    std::vector<GameObject*> children;
};
