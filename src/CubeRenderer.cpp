//
// Created by aviator on 5/5/16.
//

#include <GL/glew.h>

#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CubeRenderer.h"
#include "shader.h"

const int WIDTH = 800;
const int HEIGHT = 600;

GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, /// ! 1
        1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  /// ! 2
        1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
        1.0f, -1.0f,  0.5f,  1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,  0.0f, 1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f
};

CubeRenderer::CubeRenderer():
        cubeShader("data/shaders/cubevertex.frag", "data/shaders/cubefragment.frag"),
        cubeVertices(std::begin(vertices), std::end(vertices)),
        projectionMatrix(glm::perspective(45.0f, (GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f))
{

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    std::cout << "cubeVertices.size * sizeof(GLfloat) = " << cubeVertices.size() * sizeof(GLfloat) << std::endl;
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), &cubeVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    loadTexture("data/wall.jpg", &texture);
}

CubeRenderer::~CubeRenderer()
{
    glDeleteTextures(1, &texture);

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
}



void CubeRenderer::render(Cube& cube, Camera * camera)//Coords3f coords, Camera * camera)
{
    cubeShader.use();

    glm::mat4 model;

    float cubeScaler = cube.edgeLength() / LOCAL_EDGE_LENGTH;
    model = glm::translate(model, cube.getCenter());
    model = glm::scale(model, glm::vec3(cubeScaler, cubeScaler, cubeScaler));

    // Get their uniform location
    GLint viewLoc = glGetUniformLocation(cubeShader.getProgram(), "view");
    GLint projLoc = glGetUniformLocation(cubeShader.getProgram(), "projection");
    GLint modelLoc = glGetUniformLocation(cubeShader.getProgram(), "model");
    // Pass them to the shaders
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(vertexArrayObject);

    glActiveTexture(GL_TEXTURE0_ARB);
    glUniform1i(glGetUniformLocation(cubeShader.getProgram(), "wallTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

int CubeRenderer::loadTexture(const GLchar * imagePath, GLuint * texture)
{
    assert(imagePath);

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    sf::Image image;
    if (!image.loadFromFile(imagePath)) {
        std::cout << "Unable to load [" << imagePath << "]" << std::endl;
        return -1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

