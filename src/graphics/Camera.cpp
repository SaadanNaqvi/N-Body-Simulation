#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch){
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->movementSpeed = 10.0f;
    this->zoom = 60.0f;
    this->mouseSensitivity = 0.1f;
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio){
    float nearPlane = 0.001f;
    float farPlane  = 10000.0f;
    return glm::perspective(glm::radians(zoom), aspectRatio, nearPlane, farPlane);
}

void Camera::processKeyboard(int direction, float deltaTime){
    float velocity = movementSpeed * deltaTime;
    float zoomFactor = glm::clamp(zoom / 60.0f, 0.2f, 3.0f);
    velocity *= zoomFactor;
    
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += worldUp * velocity;
    if (direction == DOWN)
        position -= worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch){
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset){
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors(){
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}