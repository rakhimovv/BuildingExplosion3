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
#include "GameParameters.h"
#include "GameObjects/Line.h"
#include "GameObjects/Camera.h"
#include "GameObjects/Mesh.h"

class LineRenderer
{
public:
    LineRenderer(GameParameters& gameParameters);

    ~LineRenderer();

    void render(Line& line, Camera& camera);

private:
    GLuint vertexBufferObject;
    GLuint vertexArrayObject;
    GameShader lineShader;
    std::vector<GLfloat> lineVertices;

    const float LOCALSPACE_LENGTH;
};
