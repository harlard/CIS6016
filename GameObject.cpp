#include "GameObject.h"

GameObject::GameObject(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : name(name), position(position), rotation(rotation), scale(scale), mesh(nullptr)
{
}

GameObject::~GameObject()
{
    for (auto child : children) {
        delete child;
    }
}


void GameObject::updateUniforms()
{
    // Implementation here
}


void GameObject::setMesh(Mesh* mesh)
{
    this->mesh = mesh;
    if(shaderProgram != nullptr)
    {
      this->mesh->setShader(shaderProgram);
    }
}

void GameObject::setShader(ShaderProgram* shaderProgram)
{
  this->shaderProgram = shaderProgram;
  if(mesh != nullptr)
  {
    mesh->setShader(shaderProgram);
  }
}

ShaderProgram* GameObject::getShader()
{
  return shaderProgram;
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

void GameObject::updateChildren(float deltaTime)
{
  for (auto child : children) {
      child->update(deltaTime);
  }
}

void GameObject::update(float deltaTime)
{
  updateChildren(deltaTime);
}

void GameObject::draw(const glm::mat4& parentTransform, const glm::mat4& view)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);


    if (mesh && shaderProgram)
    {
        shaderProgram->use();
        updateUniforms();
        shaderProgram->setMat4("projection", parentTransform);
        shaderProgram->setMat4("model", modelMatrix);
        shaderProgram->setMat4("view", view);
        mesh->draw();
        shaderProgram->unuse();
    }

    glm::mat4 globalTransform = parentTransform * modelMatrix;

    for (auto& child : children)
    {
        child->draw(globalTransform, view);
    }


}
