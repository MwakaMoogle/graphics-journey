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
