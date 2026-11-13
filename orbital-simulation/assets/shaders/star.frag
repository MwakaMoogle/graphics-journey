#version 330 core
out vec4 fragColour;

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 objectColour;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Intensity - based on viewing angle
    float intensity = max(dot(norm, viewDir), 0.0);
    // intensity = pow(intensity, 3.0);
    
    // Edge colour of the sar - dynamicly increases the saturation/contrast
    vec3 edgeColour = pow(objectColour, vec3(2.5) * 0.75);

    // Blend from the edge colour to the object colour
    vec3 finalColour = mix(edgeColour, objectColour, intensity);

    fragColour = vec4(finalColour, 1.0);
}
