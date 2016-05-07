//
// Created by aviator on 5/7/16.
//

#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assert.h>
#include "shader.h"

class LineRenderer
{
public:
    LineRenderer();

    ~LineRenderer();

    void render(const glm::vec3& p0, const glm::vec3& p1);

private:
    GLuint vertexBufferObject;
    GLuint vertexArrayObject;
    GameShader lineShader;
    std::vector<GLfloat> lineVertices;

    const float LOCALSPACE_LENGTH;
};