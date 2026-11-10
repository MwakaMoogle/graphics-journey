// Fragement Shader 

#version 330

in vec3 postionEye;
in vec3 transformedNormal;
in vec4 baseColour;

out vec4 outputColor;

uniform uint light_mode;
uniform vec3 light_dir1;
uniform vec3 light_dir2;
uniform vec3 light_colour1;
uniform vec3 light_colour2;

struct Light {
    vec3 colour;
    vec3 dir;
};

// Calculates the diffuse light
vec4 calculateDiffuseLight(Light light, vec3 normalNormalised, vec4 diffuseColour) {
    /* Calculate the diffuse componenet
        Find the angle between the light and the normal using
        the dot product, 'dot()'. Use 'max' to prevent negative
        light values on the dark side of the object.
        */
    float diffuseComponent = max(dot(normalNormalised, normalize(light.dir)), 0.0);
    vec4 diffuseLight = diffuseComponent * diffuseColour * vec4(light.colour, 1.0f);
    return diffuseLight;
}

// Calculates the specular light
vec4 calculateSpecularLight(Light light, vec3 normalNormalised, vec3 viewDir, float shininess, vec3 colour) {
    vec3 reflectDir = reflect(normalize(-light.dir), normalNormalised);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specularLight = vec4(colour, 1.0f) * specularFactor;
    return specularLight;
}

void main(){
    // Initialise the lights
    Light light1 = Light(light_colour1, light_dir1);
    Light light2 = Light(light_colour2, light_dir2);

    // Re-normalise the interpolated vectors
    vec3 normalNormalised = normalize(transformedNormal);
    vec3 viewDir = normalize(-postionEye);


    // Ambient calculation
    vec4 ambient = baseColour * 0.2;

    // Diffuse light 
    vec4 diffuseLight = calculateDiffuseLight(light1, normalNormalised, baseColour) +
                        calculateDiffuseLight(light2, normalNormalised, baseColour);

    // Specular Light
    vec4 specularLight = calculateSpecularLight(light1, normalNormalised, viewDir, 32.0, vec3(1.0f, 1.0f, 1.0f)) +
                         calculateSpecularLight(light2, normalNormalised, viewDir, 32.0, vec3(1.0f, 1.0f, 1.0f));

    // Switch between lighting modes
    if (light_mode == 0u) {
        outputColor = baseColour;
    }
    else if (light_mode == 1u) {
        outputColor = ambient;
    }
    else if (light_mode == 2u) {
        outputColor = ambient + diffuseLight;
    }
    else {
        outputColor = ambient + diffuseLight + specularLight;
    }
}
