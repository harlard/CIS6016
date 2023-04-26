#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <string>

class Camera {
public:
    Camera(const std::string& id, const glm::vec3& position, const glm::vec3& up, const glm::vec3& front);
    void setViewport(int width, int height);

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);

    void zoom(float deltaY);
    void look(float deltaX, float deltaY);
    void move(CameraMovement direction, float deltaTime);


    void updateVectors_();


    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setFarClip(float farClip);
    void setNearClip(float nearClip);

private:
    std::string cameraId_;

    glm::vec3 position_;
    glm::vec3 rotation_;

    int viewportWidth_;
    int viewportHeight_;
    float fieldOfView_;

    float nearClip_;
    float farClip_;

    float movementSpeed_;
    float sensitivity_;

    float yaw_;
    float pitch_;
};

#endif // CAMERA_H
