/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: Light.hpp 
 * Description: Defines a PointLight struct storing spatial coordinates,
 *              colou, and intensity multipliers (ambient, diffuse, specular)
 *              to govern the lighting calculations.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct PointLight {
    // Spatial and Colour data
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Default to the origin
    glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);   // Defulat white light

    // Intensity Multipliers
    float ambientStrength = 0.1f;
    float diffuseStrength = 1.0f;
    float specularStrength = 0.5f;

};

#endif
