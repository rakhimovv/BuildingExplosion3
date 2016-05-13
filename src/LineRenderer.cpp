//
// Created by aviator on 5/7/16.

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "LineRenderer.h"
#include "GameObjects/Camera.h"

LineRenderer::LineRenderer(GameParameters& gameParameters):
        lineShader(gameParameters.GetLineVertexShader().c_str(), gameParameters.GetLineFragmentShader().c_str()),
                              LOCALSPACE_LENGTH(2.0f)
{
//    GLfloat vertices[] = {
//            -2.0f, 0.0f, 0.5f,
//            2.0f, 1.0f, 0.5f
//    };

//    lineVertices.insert(lineVertices.end(), std::begin(vertices), std::end(vertices));

    // Reserve space for vertices that will be added in the future
    // Because glBufferData has &lineVertices[0] in the parameters

    lineVertices.resize(6 * sizeof(GLfloat));
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(GLfloat), &lineVertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

LineRenderer::~LineRenderer()
{
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void LineRenderer::render(Line& line, Camera& camera)
{
    line.dump();
    lineVertices.insert(lineVertices.end(), line.getVertices().begin(), line.getVertices().end());

    glm::mat4 model;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    glm::mat4 modelViewProjection = projection * view * model;

    lineShader.use();

//    Матрица точно подается в шейдер, т.к. я пробовал менять масштаб, линия реагирует
    GLuint mvpLocation = glGetUniformLocation(lineShader.getProgram(), "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(modelViewProjection));

    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

