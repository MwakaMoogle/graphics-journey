#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Shader.hpp>
#include <Sphere.hpp>
#include <Planet.hpp>

#include <glm/glm.hpp>
#include "glm/trigonometric.hpp"

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
   
    // Create sphere master geometry
    Sphere sphereGeoemtry(64, 64);

    // Initialise planet objects
    Planet earth(&sphereGeoemtry);
    Planet moon(&sphereGeoemtry);
    
    // Planet config
    moon.transform.scale = glm::vec3(0.3f);
    moon.transform.position = glm::vec3(3.0f, 0.0f, 0.0f);


    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Clear screen 
        glClearColor(0.05f, 0.05f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate shader
        orbitalShader.use();
        
        // Camera/View and Projection Matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SRC_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        // Send the View and Projection maticies to the GPU
        orbitalShader.setMat4("projection", projection);
        orbitalShader.setMat4("view", view);
        
        // Draw objects
        earth.draw(orbitalShader);
        moon.draw(orbitalShader);

        
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


