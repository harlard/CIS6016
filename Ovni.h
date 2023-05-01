#pragma once

#include "GameObject.h"

class Ovni : public GameObject {
public:
    Ovni(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    virtual ~Ovni();
    void updateUniforms() override;
    void update(float deltaTime) override;

};
