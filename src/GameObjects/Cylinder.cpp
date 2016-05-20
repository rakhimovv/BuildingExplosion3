#include "Cylinder.h"

#include <assert.h>

#define VECTOR_PUSH_BACK(vertices, vector)  vertices.push_back(vector.x); \
                                            vertices.push_back(vector.y); \
                                            vertices.push_back(vector.y);


Cylinder::Cylinder(glm::vec3 *p0, glm::vec3 *p1, GameShader *shader, GameParameters &gameParameters)
{
    assert(p0);
    assert(p1);
    assert(shader);

    glm::vec3 o1(0.0f, 1.0f, 0.0f);
    glm::vec3 o2(0.0f, -1.0f, 0.0f);

    glm::vec3 a(-1.0f, 1.0f, 0.0f);
    glm::vec3 b(-1.0f, -1.0f, 0.0f);

    VECTOR_PUSH_BACK(cylinderVertices, a);
    VECTOR_PUSH_BACK(cylinderVertices, b);

    float radius = 0.2f;
    float angleStep = 45.0f;

    for (int i = 0; i < 4; i++) {
        glm::
    }
}

std::vector<float>& Cylinder::getVertices()
{
    return this->cylinderVertices;
}

std::vector<int>& Cylinder::getIndexes()
{
    return this->cylinderIndices;
}

void Cylinder::render(Camera& camera)
{
    // todo
}
