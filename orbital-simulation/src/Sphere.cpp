#include "Sphere.hpp"
#include <glad/glad.h>
#include <vector>
#include "cmath"

/* 
 * - Constructor - 
 * Generates the verticies, normals and indicies. 
 * Load the specific data into the GPU
 */
Sphere::Sphere(unsigned int xSegments, unsigned int ySegments) {
    std::vector<float> verticies;
    std::vector<unsigned int> indicies;
    const float PI = 3.14159265359f;

    // Generate Verticies, Colours and Normals
    for (unsigned int x = 0; x <= xSegments; ++x) {
        for (unsigned int y = 0; y <= ySegments; ++y) {
            float xSegment = (float)x / (float)xSegments;
            float ySegment = (float)y / (float)ySegments;

            /*
             * - Spherical coordinates to Cartesian conversion
             *   generates a unit sphere (Radius = 1.0) with Y-up orientation.
             *   1. The Angles:
             *      - Longitude (theta): xSegment * 2.0 * PI
             *        (Sweeps full 360 degrees / 2-Pi around the vertical axis)
             *      - Lattitude (phi): ySegment * PI
             *        (Sweeps 180 degrees / Pi fromthe North to South
             *
             *   2. The Cartesian Mapping (X, Y, Z):
             *      - Y = cos(phi)
             *        Calculates the vertical height based purely on the latitude.
             *      - X = cos(theta) * sin(phi)
             *      - Z = sin(theta) * sin(phi)
             *        The sin(phi) component determines the radius of the horizontal slice 
             *        at the current latitude. The cos(theta) and sin(theta) components 
             *        use that slice radius to plot the circle on the X/Z plane.
             */
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            // Position (Location 0)
            verticies.push_back(xPos);
            verticies.push_back(yPos);
            verticies.push_back(zPos);

            // Colour (Location 1) - Default neutral grey/white
            verticies.push_back(0.8f);
            verticies.push_back(0.8f);
            verticies.push_back(0.8f);

            // Normal (Location 2) - For a unit sphere at the origin,the postion is the normal
            verticies.push_back(xPos);
            verticies.push_back(yPos);
            verticies.push_back(zPos);
        }
    }

    // Generate the Indices
    bool oddRow = false;
    for (unsigned int y = 0; y < ySegments; ++y) {
        if (!oddRow) {
            for (unsigned int x = 0; x <= xSegments; ++x) {
                indicies.push_back(y * (xSegments + 1) + x);
                indicies.push_back((y + 1) * (xSegments + 1) + x);
            }
        } else {
            for (int x = xSegments; x >= 0; --x) {
                indicies.push_back((y + 1) * (xSegments + 1) + x);
                indicies.push_back(y * (xSegments + 1) + x);
            }
        }
        oddRow = !oddRow;
    }

    // Save the count to the class member variable
    this->indexCount = indicies.size();

    // Generate the bind to the GPU buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glad_glBindVertexArray(VAO);

    // Push vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    // Push the index data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

    /*
     * Configure the Vertex Layout
     * Tells the GPU how to interprut the giant block of floats in the VBO.
     * Since there are 10 floats pushed per vertex (3 pos + 4 col + 3 norm),
     * the 'stride' is 10 * sizeof(float)
     * Stride: Total size of bytes in a complete vertex, tells the GPU exactly
     *         how far to jump forward in the memory buffer to find the start of
     *         the next vertex.
     */
    int stride = 10 * sizeof(float); // 40 bytes
    
    // Attribute 0: Postion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Colour
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Attribute 2: Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO to prevent accidental tampering later
    glBindVertexArray(0);
}

// Draws the sphere to the screen
void Sphere::draw() const {
    // Bind the sphere's specific Vertex Array Object
    glBindVertexArray(VAO);
    
    // Draw the geometry using the Element Buffer Objects's indicies
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind 
    glBindVertexArray(0);
}
