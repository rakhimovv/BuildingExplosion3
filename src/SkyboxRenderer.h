//
// Created by aviator on 5/6/16

#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class SkyBoxRenderer
{
public:
    SkyBoxRenderer();

    void render();

private:
    std::vector<GLfloat> cubeVertices;
    GameShader cubeMapShader;
    std::vector<const GLchar *> boxFaces;
    GLuint cubeMapTexture;
    GLuint skyboxVAO;
    GLuint skyboxVBO;

    void loadCubeMap();
};
