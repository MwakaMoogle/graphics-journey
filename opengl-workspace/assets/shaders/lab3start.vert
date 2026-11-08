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
uniform vec3 light_dir;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;

//vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

void main()
{
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
    vec3 light_dir_normalised = normalize(light_dir);
    vec3 normal_normalised = normalize(transformed_normal);

    /* Calculate the diffuse componenet
       Find the angle between the light and the normal using
       the dot product, 'dot()'. Use 'max' to prevent negative
       light values on the dark side of the object.
    */
    float diffuse_component = max(dot(normal_normalised, light_dir_normalised), 0.0);

    // Calculate the diffuse lighting
    vec4 diffuse_light = diffuse_component * diffuse_colour;

    // Transform the vertex postion into eye space (reletive to the camera)
    vec4 position_eye = model_view * position_h;

    // Calculate the vector pointing form the vertex to the camera
    vec3 view_dir = normalize(-position_eye.xyz);

    // Calculate the Reflection Factor
    vec3 reflect_dir = reflect(-light_dir_normalised, normal_normalised);

    // Calculate the Specular Component
    float shininess = 32.0;
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    
    vec4 specular_light = vec4(1.0, 1.0, 1.0, 1.0) * specular_factor;

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

