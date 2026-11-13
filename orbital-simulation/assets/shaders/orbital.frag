/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: orbital.vert 
 * Description: Minimal fallback unlit shader. Projects verticies into space
 *              and outputs a flat, uniform colour.
 */
#version 330 core 
out vec4 fragColour;

in vec4 vertexColour;

uniform vec3 objectColour;

void main() {
    fragColour = vec4(objectColour, 1.0f);
}
