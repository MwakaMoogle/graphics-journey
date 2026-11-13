/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: phong.vert 
 * Description: Implements the Phong reflection model. Calculates directinoal
 *              shadows, ambient light, and specular hifhlights for opaque bodies.
 */

#version 330 core 
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);

    fragPos = vec3(model * vec4(pos, 1.0)); // 3D world postion    
    fragNormal = mat3(transpose(inverse(model))) * normal; // Normal Vector
}
