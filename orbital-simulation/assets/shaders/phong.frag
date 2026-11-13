/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: phong.frag 
 * Description: Implements the Phong reflection model. Calculates directinoal
 *              shadows, ambient light, and specular hifhlights for opaque bodies.
 */
#version 330 core
out vec4 fragColour;

in vec3 fragPos;
in vec3 fragNormal;

// Materrial Varaibles
uniform vec3 objectColour;
uniform float shininess;

// Light variables
struct PointLight {
    vec3 position;
    vec3 colour;
};
uniform vec3 viewPos;

// Set a maximum limit
#define MAX_LIGHTS 4
uniform PointLight lights[MAX_LIGHTS];
uniform int numLights; // Number of active lights

void main(){
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 totalLight = vec3(0.0);

    // Loop through however many lights are currently active
    for (int i = 0; i < numLights; i++) {
        // Ambient
        vec3 ambient = ambientStrength * lights[i].colour;

        // Diffuse
        vec3 lightDir = normalize(lights[i].position - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].colour;

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lights[i].colour;

        // Accumulate
        totalLight += (ambient + diffuse + specular);
    }
    // Combine all three components
    vec3 result = totalLight * objectColour;
    fragColour = vec4(result, 1.0);
}
