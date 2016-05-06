//
// Created by aviator on 5/5/16.
//

#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include <glm/glm.hpp>

class Cube
{
public:
    Cube();
    Cube(glm::vec3 cubeCenter, float edgeLength);

    glm::vec3 getCenter()
    {
        return this->cubeCenter;
    }

    float edgeLength()
    {
        return edge;
    }

private:
    glm::vec3 cubeCenter;
    float edge;
};

#endif