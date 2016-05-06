//
// Created by aviator on 5/5/16.
//

#include "Cube.h"

#include <glm/glm.hpp>
#include <assert.h>


Cube::Cube(glm::vec3 centerCoords, float edgeLength): cubeCenter(centerCoords)
{
    this->edge = edgeLength;

    assert(this->cubeCenter.x == centerCoords.x);
    assert(this->cubeCenter.y == centerCoords.y);
    assert(this->cubeCenter.z == centerCoords.z);

    assert(this->edge > 0);
}