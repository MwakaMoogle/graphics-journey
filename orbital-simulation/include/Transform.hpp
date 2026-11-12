#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    // Default data
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    // Calculates model matrix for an object
    glm::mat4 getModelMatrix() const {
        glm::mat4 model= glm::mat4(1.0f); // Identity matrix
        
        // Translate
        model = glm::translate(model, position);

        // Rotate
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        // Scale
        model = glm::scale(model, scale);

        return model;
    }
};

#endif
