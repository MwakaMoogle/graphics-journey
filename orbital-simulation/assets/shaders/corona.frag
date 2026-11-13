/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: corona.frag 
 * Description: Transparent optical halo shader. Uses an inverted view-angle
 *              calculation to generate an alpha gradient for additive blending.
 */
#version 330 core
out vec4 fragColour;

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 objectColour;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    float dotProduct = max(dot(norm, viewDir), 0.0);
    float alpha = pow(dotProduct, 3.0);
    
    fragColour = vec4(objectColour, alpha);
}
