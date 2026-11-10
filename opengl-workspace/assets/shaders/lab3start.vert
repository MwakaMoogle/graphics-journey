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
uniform uint light_mode;
uniform vec3 light_dir1;
uniform vec3 light_dir2;
uniform vec3 light_colour1;
uniform vec3 light_colour2;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;

//vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

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
vec4 calculateSpecularLight(Light light, vec3 normalNormalised, vec3 viewDir ,float shininess, vec3 colour) {
    vec3 reflectDir = reflect(-light.dir, normalNormalised);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specularLight = vec4(colour, 1.0f) * specularFactor;
    return specularLight;
}

void main()
{
    
    Light light1 = Light(light_colour1, light_dir1);
    Light light2 = Light(light_colour2, light_dir2);
    
	vec4 diffuse_colour;
	vec4 position_h = vec4(position, 1.0);
	
	if (colourmode == 1u)
		diffuse_colour = colour;
	else
		diffuse_colour = vec4(0.0, 1.0, 0, 1.0);

	// Define the vertex colour
	// Old: fcolour = diffuse_colour

    // Adding ambient light
    vec4 ambient = diffuse_colour * 0.2;

    // Model view matrix
    mat4 model_view = view * model;

    // Normal Matrix
    mat3 normal_matrix = transpose(inverse(mat3(model_view)));

    // Transform the model
    vec3 transformed_normal = normal_matrix * normal;

    // Normalise the vectors
    vec3 normal_normalised = normalize(transformed_normal);

    // Transform the vertex postion into eye space (reletive to the camera)
    vec4 position_eye = model_view * position_h;

    // Calculate the vector pointing form the vertex to the camera
    vec3 view_dir = normalize(-position_eye.xyz);
    
    // Diffuse Light
    vec4 diffuse_light = calculateDiffuseLight(light1, normal_normalised, diffuse_colour) + calculateDiffuseLight(light2, normal_normalised, diffuse_colour);

    // Specular Light
    vec4 specular_light = calculateSpecularLight(light1, normal_normalised, view_dir, 8.0, vec3(1.0f, 1.0f, 1.0f)) + calculateSpecularLight(light2, normal_normalised, view_dir, 8.0, vec3(1.0f, 1.0f, 1.0f));

    // Switch between lighting modes
    if (light_mode == 0u) {
        fcolour = diffuse_colour;
    }
    else if (light_mode == 1u) {
        fcolour = ambient;
    }
    else if (light_mode == 2u) {
        fcolour = ambient + diffuse_light;
    }
    else {
        fcolour = ambient + diffuse_light + specular_light;
    }

    // Define the vertex position
	gl_Position = projection * view * model * position_h;
}

