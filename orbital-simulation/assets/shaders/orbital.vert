/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: orbital.vert 
 * Description: Minimal fallback unlit shader. Projects verticies into space
 *              and outputs a flat, uniform colour.
 */

#version 330 core 
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

out vec4 vertexColour;

// Matricies for 3d transformation
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vertexColour = colour;
}
