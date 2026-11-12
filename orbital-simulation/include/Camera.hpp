#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        // Camera attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        // Angles 
        float yaw;
        float pitch;

        // Camera options
        float movementSpeed = 5.0f;
        float mouseSensitivity = 0.1f;

        // Constuctor 
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f)): 
            front(glm::vec3(0.0f, 0.0f, -1.0f)), yaw(-90.0f), pitch(0.0f) {
                this->position = position;
                worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
                updateCameraVectors(); 
        };

        // Methods

        // Returns the view matix
        glm::mat4 getViewMatrix() {
            return glm::lookAt(position, position + front, up);
        }

        // Processes keyboard input
        void ProcessKeyboard(int direction, float deltaTime) {
            float velocity = movementSpeed * deltaTime;
            if (direction == 0) position += front * velocity; // Forward
            if (direction == 1) position -= front * velocity; // Backward
            if (direction == 2) position -= right * velocity; // Left
            if (direction == 3) position += right * velocity; // Right
        }

        // Proccess mouse movement 
        void processMouseMovement(float xOffset, float yOffset) {
            xOffset *= mouseSensitivity;
            yOffset *= mouseSensitivity;

            yaw += xOffset;
            pitch += yOffset;

            // Constrain pitch to avoid screen flipping
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            updateCameraVectors();
        }


    private:
        // Calculates the front vector for the Camera's updataed angles
        void updateCameraVectors() {
            glm::vec3 newFront;
            newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            newFront.y = sin(glm::radians(pitch));
            newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(newFront);

            // Recalculate Right and Up vector
            right = glm::normalize(glm::cross(front, worldUp));
            up = glm::normalize(glm::cross(right, front));
        }
};

#endif
