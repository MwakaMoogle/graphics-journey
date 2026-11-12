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
    sphere->draw();
};
