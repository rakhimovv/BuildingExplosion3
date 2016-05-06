#pragma once

#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "Cube.h"
#include "shader.h"

class CubeRenderer
{
public:
    CubeRenderer ();

    ~CubeRenderer ();

    void render(Cube& cube);

private:
    GameShader cubeShader;
    std::vector<GLfloat> cubeVertices;
    GLuint  vertexBufferObject;
    GLuint  vertexArrayObject;
    GLuint  texture;

    const float LOCAL_EDGE_LENGTH = 2;

    int loadTexture(const GLchar * imagePath, GLuint * texture);
};
