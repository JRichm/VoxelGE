#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    glm::mat4 getViewMatrix();

    void processKeyboard(Camera_Movement direction, bool shifting, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

#endif