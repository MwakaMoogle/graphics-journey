#version 330 core 
out vec4 fragColour;

in vec4 vertexColour;

uniform vec3 objectColour;

void main() {
    fragColour = vec4(objectColour, 1.0f);
}
