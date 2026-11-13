/*
 * Module: CS51012 - GPU Programming and real-time rendering
 * Assignment: Individual Program
 * File: Sphere.hpp 
 * Description: Procedural geometry generator. Calculates vertex positions,
 *              normals, and indicies for a 3D mathematical sphere and manages
 *              its Vertex Array Object (VAO) state.
 */

#ifndef SPHERE_H
#define SPHERE_H

class Sphere {
    public:
        // Constructor
        Sphere(unsigned int xSegments, unsigned int ySegments);

        // Functions 
        void draw() const;

    private:
        unsigned int VAO;           // Stores all the attribute settings
        unsigned int VBO;           // Buffer storing the raw data (postions, colours, normals)
        unsigned int EBO;           // Buffer storing the indicies (how the verticies connect into triangles)
        unsigned int indexCount;    // How many triangles points to draw
};

#endif
