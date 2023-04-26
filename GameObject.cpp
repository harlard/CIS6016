#include "GameObject.h"

GameObject::GameObject(const std::string& name)
    : name(name), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), mesh(nullptr)
{
}

GameObject::~GameObject()
{
    for (auto child : children) {
        delete child;
    }
}

void GameObject::setPosition(const glm::vec3& position)
{
    this->position = position;
}

glm::vec3 GameObject::getPosition() const
{
    return position;
}

void GameObject::setRotation(const glm::vec3& rotation)
{
    this->rotation = rotation;
}

glm::vec3 GameObject::getRotation() const
{
    return rotation;
}

void GameObject::setScale(const glm::vec3& scale)
{
    this->scale = scale;
}

glm::vec3 GameObject::getScale() const
{
    return scale;
}

void GameObject::setMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

Mesh* GameObject::getMesh() const
{
    return mesh;
}

std::string GameObject::getName() const{
  return name;
}

void GameObject::setName(const std::string& name){
  this->name = name;
}

void GameObject::addChild(GameObject* child)
{
    children.push_back(child);
}

void GameObject::removeChild(GameObject* child)
{
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            break;
        }
    }
}

void GameObject::update(float deltaTime)
{
    for (auto child : children) {
        child->update(deltaTime);
    }
}

void GameObject::draw(const glm::mat4& parentTransform, const glm::mat4& view)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position_);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale_);

    glm::mat4 globalTransform = parentTransform * modelMatrix;

    if (mesh_ && shaderProgram_)
    {
        shaderProgram->use();
        shaderProgram->setMat4("model", globalTransform);
        shaderProgram->setMat4("view", view);
        mesh->draw();
    }

    for (auto& child : children_)
    {
        child->draw(globalTransform, view);
    }
}
