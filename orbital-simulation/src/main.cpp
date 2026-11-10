#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.hpp>
#include <Sphere.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const unsigned int SCR_WIDTH = 1024;
const unsigned int SRC_HEIGHT = 768;

// Funciton prototypes
GLFWwindow* initialiseAPI();
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);


// Main funciton
int main() {
    // Setup window and OpenGl context
    GLFWwindow* window = initialiseAPI();
    if (!window) {
        return -1;
    }

    // Load Shaders
    Shader orbitalShader("../assets/shaders/orbital.vert", "../assets/shaders/orbital.frag");
    
    Sphere earth(64, 64); // Initialise a sphere object

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Clear screen 
        glClearColor(0.05f, 0.05f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // --- DRAWING AND MATRIX MATH ---
        orbitalShader.use();
        
        // Projection Matrix 
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
        // View Matrix (The Camera)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
        // Model Matrix
        glm::mat4 model = glm::mat4(1.0f);
        
        // Send matricies to the GPU
        unsigned int projLoc = glGetUniformLocation(orbitalShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(orbitalShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int modelLoc = glGetUniformLocation(orbitalShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        earth.draw(); // Draw the sphere
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;

}

// Initialisation window
GLFWwindow* initialiseAPI() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW\n";
        return nullptr;
    }
    
    // Configure for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SRC_HEIGHT, "Orbital Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
   
    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD\n";
        return nullptr;
    }

    return window;
}

// Process input (Continuos Polling)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Callback for window resising
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


