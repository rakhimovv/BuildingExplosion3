//
// Created by aviator on 5/7/16.
//

#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assert.h>

class LineRenderer
{
public:
    LineRenderer();

    ~LineRenderer();

    void render(glm::vec3 * p0, glm::vec3 * p1);

private:
    GLuint vertexBufferObject;
    GLuint vertexArrayObject;
};