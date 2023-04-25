#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(float deltaTime);
void shipControl(float deltaTime);
void setCamaraPos(glm::vec3 pos);
void inputEntry(float deltaTime);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 GetPosDelta();
glm::vec3 GetDirection();

#endif
