#version 330 core 
out vec4 fragColour;

in vec4 vertexColour;

void main() {
    fragColour = vertexColour;
}
