#pragma once

#include "../shader.h"
#include "../GameParameters.h"
#include "Camera.h"
#include "Mesh.h"

#include <vector>
#include <glm/glm.hpp>

class Cylinder : public Mesh {
public:
    Cylinder(glm::vec3 * p0, glm::vec3 * p1, GameShader * shader, GameParameters& gameParameters);
    ~Cylinder();

    //! @overload
    virtual std::vector<float>& getVertices();
    //! @overload
    virtual std::vector<int>& getIndexes();
    //! @overload
    void render(Camera& camera);

private:
    std::vector<float> cylinderVertices;
    std::vector<int> cylinderIndices;

    glm::vec3 pointOne;
    glm::vec3 pointTwo;
    float     cylinderRadius;

    GameShader * cylinderShader;
};
