#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Shader.hpp>
#include <Sphere.hpp>
#include <Planet.hpp>
#include <Camera.hpp>
#include <Light.hpp>

#include <glm/glm.hpp>
#include "glm/trigonometric.hpp"

// Window dimensions
unsigned int scr_width = 1024;
unsigned int scr_height = 768;

// Time and Frames
float deltaTime = 0.0f; // Time between current and last frame
float lastFrame = 0.0f; // Time of last frame
int nbFrames = 0;
float lastTime = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;

// Colours
const glm::vec3 earthBlue = glm::vec3(40.0f / 255.0f, 122.0f / 255.0f, 184.0f / 255.0f);
const glm::vec3 moonGrey = glm::vec3(207.0f / 255.0f, 234.0f / 255.0f, 233.0f / 255.0f);

// Funciton prototypes
GLFWwindow* initialiseAPI();
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void displayFPS(GLFWwindow* window, float currentFrame);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);


// Main funciton
int main() {
    // Setup window and OpenGl context
    GLFWwindow* window = initialiseAPI();
    if (!window) {
        return -1;
    }

    // Load Shaders
    Shader orbitalShader("../assets/shaders/orbital.vert", "../assets/shaders/orbital.frag");
    Shader phongShader("../assets/shaders/phong.vert", "../assets/shaders/phong.frag");
    PointLight sun;

    // Create sphere master geometry
    Sphere sphereGeoemtry(64, 64);

    // Initialise planet objects
    Planet sunModel(&sphereGeoemtry);
    Planet earth(&sphereGeoemtry);
    Planet moon(&sphereGeoemtry);

    // Sun config
    sunModel.colour = glm::vec3(1.0f, 0.9, 0.1f); // Bright yellow/white
    
    // Earth config
    earth.colour = earthBlue;
    earth.shininess = 64.0f;
    earth.rotationSpeed = 0.5f;
    earth.orbitalRadius = 6.0f;
    earth.orbitalSpeed = 0.8f;

    // Moon config
    moon.colour = moonGrey;
    moon.shininess = 4.0f;
    moon.transform.scale = glm::vec3(0.3f);
    moon.rotationSpeed = 1.0f;
    moon.orbitalRadius = 2.0f;
    moon.orbitalSpeed = 2.5f;

    // Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate Delta time 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Display FPS
        displayFPS(window, currentFrame);

        // Input
        processInput(window);

        // Clear screen 
        glClearColor(0.05f, 0.05f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update
        sunModel.update(deltaTime, glm::vec3(0.0f));
        earth.update(deltaTime, sunModel.transform.position);
        moon.update(deltaTime, earth.transform.position);
        
        // Activate shaders
       
        phongShader.use();
        
        // Camera/View and Projection Matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        // Draw unlit objects
        orbitalShader.use(); 
        orbitalShader.setMat4("projection", projection);
        orbitalShader.setMat4("view", view);

        sunModel.draw(orbitalShader);

        // Draw lit objects
        phongShader.use();
        phongShader.setMat4("projection", projection);
        phongShader.setMat4("view", view);

        // Send light uniforms
        phongShader.setVec3("viewPos", camera.position);
        phongShader.setVec3("lightPos",sun.position);
        phongShader.setVec3("lightColour", sun.colour);
 
        earth.draw(phongShader);
        moon.draw(phongShader);

        
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
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Orbital Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
   
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

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(0, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(1, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(2, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(3, deltaTime);
    }
}

// Callback for window resising
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height > 0 ? height : 1; // Prevents division by 0, if window is minimised
}

// Calculate and display FPS
void displayFPS(GLFWwindow* window, float currentFrame) {
    nbFrames++;
    if (currentFrame - lastTime >= 1.0f) {
        std::string title = "Orbital Simulation - FPS: " + std::to_string(nbFrames);
        glfwSetWindowTitle(window, title.c_str());
        nbFrames = 0;
        lastTime = currentFrame;
    }
}

// Feeds the mouse offsets to the camera
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset);
}
