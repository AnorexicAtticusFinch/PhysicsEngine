#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum movementDirections
{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};

class Camera
{
    private:

        bool firstMouse;

        float pitch;
        float yaw;

        float speed;
        float sensitivity;

        float xLastPos;
        float yLastPos;

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;

        glm::vec3 worldUp;

        void calculateVectors();

    public:

        Camera(float speed, float sens, glm::vec3 position, glm::vec3 front, glm::vec3 worldUp);
        Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp);

        glm::mat4 getViewMatrix();

        float getPitch();
        float getYaw();

        float getSpeed();
        float getSensitivity();

        void setSpeed(float newSpd);
        void setSensitivity(float newSens);

        glm::vec3 getPos();
        glm::vec3 getFront();
        glm::vec3 getUp();
        glm::vec3 getRight();

        void keyboard_input(movementDirections direction, float deltaTime);
        void mouse_movement(float xPos, float yPos);
};

void Camera::calculateVectors()
{
    front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = -1 * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(float speed, float sens, glm::vec3 position, glm::vec3 front, glm::vec3 worldUp)
{
    firstMouse = true;

    pitch = 0;
    yaw = 0;

    this->speed = speed;
    this->sensitivity = sensitivity;
    
    this->position = position;
    this->front = front;
    this->worldUp = worldUp;

    calculateVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp)
{
    firstMouse = true;
    
    pitch = 0;
    yaw = 0;

    speed = 5;
    sensitivity = 0.05;

    this->position = position;
    this->front = front;
    this->worldUp = worldUp;

    calculateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

float Camera::getPitch()
{
    return pitch;
}

float Camera::getYaw()
{
    return yaw;
}

float Camera::getSpeed()
{
    return speed;
}

float Camera::getSensitivity()
{
    return sensitivity;
}

void Camera::setSpeed(float newSpd)
{
    speed = newSpd;
}

void Camera::setSensitivity(float newSensitivity)
{
    sensitivity = newSensitivity;
}

glm::vec3 Camera::getPos()
{
    return position;
}

glm::vec3 Camera::getFront()
{
    return front;
}

glm::vec3 Camera::getUp()
{
    return up;
}

glm::vec3 Camera::getRight()
{
    return right;
}

void Camera::keyboard_input(movementDirections direction, float deltaTime)
{
    float velocity = speed * deltaTime;

    if (direction == FORWARD)
        position += front * velocity;
    
    else if (direction == BACKWARD)
        position -= front * velocity;

    else if (direction == RIGHT)
        position += right * velocity;
    
    else if (direction == LEFT)
        position -= right * velocity;
}

void Camera::mouse_movement(float xPos, float yPos)
{
    if (firstMouse)
    {
        xLastPos = xPos;
        yLastPos = yPos;
        firstMouse = false;
    }

    float xChange = xPos - xLastPos;
    float yChange = -1 * (yPos - yLastPos);

    xLastPos = xPos;
    yLastPos = yPos;

    xChange *= sensitivity;
    yChange *= sensitivity;

    pitch += yChange;
    yaw += xChange;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    calculateVectors();
}

#endif