#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Cube.h"
#include "shader.h"
#include "GameObjects/Camera.h"
#include "GameParameters.h"

class CubeRenderer {
public:
    CubeRenderer(GameShader * shader, GameParameters &gameParameters);

    ~CubeRenderer();

    void render(Cube &cube, Camera *camera);

private:
    GameShader * cubeShader;
    std::vector<GLfloat> cubeVertices;
    GLuint vertexBufferObject;
    GLuint vertexArrayObject;
    GLuint texture;

    const glm::mat4 projectionMatrix;
    const float LOCAL_EDGE_LENGTH = 2;

    int loadTexture(const GLchar *imagePath, GLuint *texture);
};
