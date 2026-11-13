#ifndef PLANET_H
#define PLANET_H

#include <Transform.hpp>
#include <Sphere.hpp>
#include <Shader.hpp>

class Planet {
    public:
        Transform transform;
        glm::vec3 colour = glm::vec3(1.0f); // Default to white
        float shininess = 32.0f; // Defualt
        float rotationSpeed;
        float orbitalAngle;
        float orbitalRadius;
        float orbitalSpeed;

        // Constructor
        Planet(Sphere* sP);

        // Mathods
        void draw(const Shader& shader) const;
        void update(float deltaTime, glm::vec3 centerPoint);

    private:
        Sphere* sphere;
};

#endif
