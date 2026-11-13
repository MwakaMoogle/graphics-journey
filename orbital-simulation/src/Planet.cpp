#include "Shader.hpp"
#include "Sphere.hpp"
#include <Planet.hpp>

// Constructor
Planet::Planet(Sphere* sP) {
    sphere = sP;
};


// Methods
void Planet::draw(const Shader& shader) const {
    glm::mat4 model = transform.getModelMatrix();
    shader.setMat4("model", model);
    shader.setVec3("objectColour", this->colour);
    shader.setFloat("shininess", this->shininess);
    sphere->draw();
};

void Planet::update(float deltaTime, glm::vec3 centerPoint) {
    // Planet Rotation
    transform.rotation.y += rotationSpeed * deltaTime;

    if (orbitalSpeed > 0.0f) {
        orbitalAngle += orbitalSpeed * deltaTime;
    
        // Orbit -Unit Circle Mapping
        transform.position.x = centerPoint.x + (std::cos(orbitalAngle) * orbitalRadius);
        transform.position.z = centerPoint.z + (std::sin(orbitalAngle) * orbitalRadius);

        // Keep Y reletive to center
        transform.position.y = centerPoint.y;
    }
};
