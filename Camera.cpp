#include "Camera.h"

Camera::Camera(const std::string& id, const glm::vec3& position, const glm::vec3& up, const glm::vec3& front)
    : cameraId_(id), position_(position), rotation_(glm::vec3(0.0f)), viewportWidth_(0), viewportHeight_(0),
      fieldOfView_(glm::radians(45.0f)), nearClip_(0.1f), farClip_(100.0f),
      movementSpeed_(1.0f), sensitivity_(0.1f), yaw_(0.0f), pitch_(0.0f),
      front_(front), right_(glm::normalize(glm::cross(front_, up))), up_(glm::normalize(glm::cross(right_, front_))), worldUp_(up)
    {
      updateVectors_();
}

glm::vec3 Camera::getPosition() const
{
    return position_;
}

glm::vec3 Camera::getRotation() const
{
    return rotation_;
}

void Camera::setPosition(const glm::vec3& position)
{
    position_ = position;
}

void Camera::setRotation(const glm::vec3& rotation)
{
    rotation_ = rotation;
    updateVectors_();
}

void Camera::setViewport(int width, int height)
{
    viewportWidth_ = width;
    viewportHeight_ = height;
}

void Camera::setNearClip(float nearClip)
{
    nearClip_ = nearClip;
}

void Camera::setFarClip(float farClip)
{
    farClip_ = farClip;
}

float Camera::getNearClip() const
{
    return nearClip_;
}

float Camera::getFarClip() const
{
    return farClip_;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(fieldOfView_, static_cast<float>(viewportWidth_) / viewportHeight_, nearClip_, farClip_);
}

std::string Camera::getCameraId() const
{
  return cameraId_;
}

void Camera::move(const glm::vec3& direction, float deltaTime)
{
    position_ += direction * movementSpeed_ * deltaTime;
}

void Camera::lookAt(const glm::vec3& target) {
    glm::vec3 newFront = glm::normalize(target - position_);
    glm::vec3 newRight = glm::normalize(glm::cross(worldUp_, newFront));
    glm::vec3 newUp = glm::normalize(glm::cross(newFront, newRight));

    front_ = newFront;
    right_ = newRight;
    up_ = newUp;

    yaw_ = glm::degrees(std::atan2(newFront.z, newFront.x));
    pitch_ = glm::degrees(std::asin(newFront.y));

    updateVectors_();
}


void Camera::look(float deltaX, float deltaY)
{
    deltaX *= sensitivity_;
    deltaY *= sensitivity_;

    yaw_ += deltaX;
    pitch_ += deltaY;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    updateVectors_();
}

void Camera::zoom(float deltaY)
{
    if (fieldOfView_ >= glm::radians(1.0f) && fieldOfView_ <= glm::radians(45.0f))
        fieldOfView_ -= deltaY;
    if (fieldOfView_ < glm::radians(1.0f))
        fieldOfView_ = glm::radians(1.0f);
    if (fieldOfView_ > glm::radians(45.0f))
        fieldOfView_ = glm::radians(45.0f);
}

void Camera::updateVectors_()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}
