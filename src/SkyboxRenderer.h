//
// Created by aviator on 5/6/16

#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObjects/Camera.h"
#include "shader.h"
#include "GameParameters.h"

class SkyBoxRenderer
{
public:
    SkyBoxRenderer(GameShader * shader, GameParameters& gameParameters);

    void render(Camera * gameCamera);

private:
    std::vector<GLfloat> cubeVertices;

    GameShader * cubeMapShader;

    glm::mat4 modelMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    std::vector<const GLchar *> boxFaces;
    GLuint cubeMapTexture;
    GLuint skyboxVAO;
    GLuint skyboxVBO;

    void loadCubeMap();
};
