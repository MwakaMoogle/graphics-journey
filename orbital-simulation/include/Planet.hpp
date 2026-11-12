#ifndef PLANET_H
#define PLANET_H

#include <Transform.hpp>
#include <Sphere.hpp>
#include <Shader.hpp>

class Planet {
    public:
        Transform transform;

        // Constructor
        Planet(Sphere* sP);

        // Mathods
        void draw(const Shader& shader) const;

    private:
        Sphere* sphere;
};

#endif
