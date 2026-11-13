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
uniform vec3 lightPos;
uniform vec3 lightColour;
uniform vec3 viewPos;

void main(){
    // Ambient Lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColour;

    // Diffuse Lighting
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    // Calculate the dot product between the normal and light direction
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

    // Specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // Calculate how closely the reflection alligns with the camera view
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColour;

    // Combine all three components
    vec3 result = (ambient + diffuse + specular) * objectColour;
    fragColour = vec4(result, 1.0);
}
