// Starter vertex shader for lab3
// THe goal is to update this shader to implement Gourand shading
// which is per-vertex lighting

#version 330

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
uniform uint colourmode;

// Out variables to pass to the fragment shader
out vec3 postionEye;
out vec3 transformedNormal;
out vec4 baseColour;

void main(){
    vec4 position_h = vec4(position, 1.0);

    //Determin the base colour
    if (colourmode == 1u)
        baseColour = colour;
    else
        baseColour = vec4(0.0, 1.0, 0.0, 1.0);

    // Calculate transformations
    mat4 modelView = view * model;
    mat3 normalMatrix = transpose(inverse(mat3(modelView)));

    // Assign directily to the out variables
    transformedNormal = normalMatrix * normal;
    postionEye = (modelView * position_h).xyz;

    // Define the vertex position
    gl_Position = projection * view * model * position_h;
}
