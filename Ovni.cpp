
#include "Ovni.h"

Ovni::Ovni(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : GameObject(name, position, rotation, scale)
{
    // Create a new mesh for the cube object
    Mesh* mesh = new Mesh("Assets/Model/Ovni/Ovni.dae");
    setMesh(mesh);

    // Set the default shader for the cube object
    ShaderProgram* newShaderProgram = new ShaderProgram("Shaders/StandardShading.vertexshader", "Shaders/StandardShading.fragmentshader");
    setShader(newShaderProgram);
}

Ovni::~Ovni()
{
  if(mesh)delete mesh;
  if(shaderProgram)delete shaderProgram;
}

void Ovni::updateUniforms(){
  shaderProgram->setVec3("objectColor", glm::vec3(1.0f,1.0f,1.0d));
}
void Ovni::update(float deltaTime){

}
