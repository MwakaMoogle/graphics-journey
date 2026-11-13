/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: main.cpp
 * Description: Application entry point. Handles GLFW window initialisation,
 *              input polling, the main render loop, and multipass
 *              rendering pipeline for the orbital simulation
 */

#include <cmath>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Shader.hpp>
#include <Sphere.hpp>
#include <Planet.hpp>
#include <Camera.hpp>
#include <Light.hpp>

#include <glm/glm.hpp>

// Funciton prototypes
GLFWwindow* initialiseAPI();
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void displayFPS(GLFWwindow* window, float currentFrame);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
glm::vec3 normaliseRGB(glm::vec3 rgbValue);

// Window dimensions
unsigned int scr_width = 1024;
unsigned int scr_height = 768;

// Time and Frames
float deltaTime = 0.0f; // Time between current and last frame
float lastFrame = 0.0f; // Time of last frame
int nbFrames = 0;
float lastTime = 0.0f;
float simulationSpeed = 1.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;

// Colours
const glm::vec3 earthBlue = normaliseRGB(glm::vec3(40.0f, 122.0f, 184.0f));
const glm::vec3 moonGrey = normaliseRGB(glm::vec3(207.0f, 234.0f, 233.0f));
const glm::vec3 brightSun = normaliseRGB(glm::vec3(254.0f, 211.0f, 60.0f));
const glm::vec3 starlightBlue = normaliseRGB(glm::vec3(38.0f, 132.0f, 255.0f));
const glm::vec3 fireryOrange = normaliseRGB(glm::vec3(232.0f, 86.0f, 7.0f));

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
    Shader starShader("../assets/shaders/star.vert", "../assets/shaders/star.frag");
    Shader coronaShader("../assets/shaders/corona.vert", "../assets/shaders/corona.frag");
    PointLight sun;
    PointLight sun2;

    // Create sphere master geometry
    Sphere sphereGeoemtry(64, 64);

    // Initialise planet objects
    Planet sunModel(&sphereGeoemtry);
    Planet sunModel2(&sphereGeoemtry);
    Planet sunCorona(&sphereGeoemtry);
    Planet earth(&sphereGeoemtry);
    Planet moon(&sphereGeoemtry);

    // Sun config
    sunModel.colour =  brightSun;
    sunModel.transform.scale = glm::vec3(2.0f);

    // Sun2
    sunModel2.colour = starlightBlue;
    sunModel2.transform.scale = glm::vec3(1.5f);
    sunModel2.rotationSpeed = 0.2f;
    sunModel2.orbitalRadius = 16.0f;
    sunModel2.orbitalSpeed = 0.3f;

    // Corona config
    sunCorona.colour = fireryOrange;
    sunCorona.transform.scale = sunModel.transform.scale * 1.25f;
    
    // Earth config
    earth.colour = earthBlue;
    earth.shininess = 64.0f;
    earth.rotationSpeed = 0.5f;
    earth.orbitalRadius = 12.0f;
    earth.orbitalSpeed = 0.8f;

    // Moon config
    moon.colour = moonGrey;
    moon.shininess = 4.0f;
    moon.transform.scale = glm::vec3(0.27f);
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
        float scaledDelta = deltaTime * simulationSpeed;
        sunModel.update(scaledDelta, glm::vec3(0.0f));
        sunModel2.update(scaledDelta, sunModel.transform.position);
        sunCorona.update(scaledDelta, glm::vec3(0.0f));
        earth.update(scaledDelta, sunModel.transform.position);
        moon.update(scaledDelta, earth.transform.position);

        sun.position = sunModel.transform.position;
        sun.colour = sunModel.colour;
        sun2.position = sunModel2.transform.position;
        sun2.colour = sunModel2.colour;
        
        // Camera/View and Projection Matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        // Draw unlit objects
        starShader.use(); 
        starShader.setMat4("projection", projection);
        starShader.setMat4("view", view);
        starShader.setVec3("viewPos", camera.position);

        sunModel.draw(starShader);
        sunModel2.draw(starShader);

        // Draw lit objects
        phongShader.use();
        phongShader.setMat4("projection", projection);
        phongShader.setMat4("view", view);

        // Send light uniforms
        phongShader.setVec3("viewPos", camera.position);

        phongShader.setInt("numLights", 2);
        
        phongShader.setVec3("lights[0].position", sun.position);
        phongShader.setVec3("lights[0].colour", sun.colour);

        phongShader.setVec3("lights[1].position", sun2.position);
        phongShader.setVec3("lights[1].colour", sun2.colour);
 
        earth.draw(phongShader);
        moon.draw(phongShader);
        
        // Draw transparent objects
        glEnable(GL_BLEND);                 // Turn blending on
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);              // Disable depth writing

        coronaShader.use();
        coronaShader.setMat4("projection", projection);
        coronaShader.setMat4("view", view);
        coronaShader.setVec3("viewPos", camera.position);

        sunCorona.draw(coronaShader);

        glDepthMask(GL_TRUE);               // Re-enable depth writing
        glDisable(GL_BLEND);                // Turn blending off 

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
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "OpenGL Orbital Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(4, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.ProcessKeyboard(5, deltaTime);
    }   

    // Time Scale Controls
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        simulationSpeed += 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        simulationSpeed -= 1.0f * deltaTime;
        if (simulationSpeed < 0.0f) simulationSpeed = 0.0f; // Stops time from goin backward
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        simulationSpeed = 1.0f;
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
        char speedStr[10];
        snprintf(speedStr, sizeof(speedStr), "%.1fx", simulationSpeed);

        std::string title = "OpenGL Orbital Simulation - FPS: " + std::to_string(nbFrames) + " | Speed: " + speedStr;
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

// Normalises RGB values
glm::vec3 normaliseRGB(glm::vec3 rgbValue) {
    float nR = rgbValue.x / 255.0f;
    float nG = rgbValue.y / 255.0f;
    float nB = rgbValue.b / 255.0f;
    return glm::vec3(nR, nG, nB);
};
