//
// Created by aviator on 5/7/16.


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "LineRenderer.h"

LineRenderer::LineRenderer(): lineShader("data/shaders/linevertex.frag", "data/shaders/linefragment.frag"),
                              LOCALSPACE_LENGTH(2.0f)
{
    GLfloat vertices[] = {
            -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f
    };

    lineVertices.insert(lineVertices.end(), std::begin(vertices), std::end(vertices));

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(GLfloat), &lineVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

LineRenderer::~LineRenderer()
{
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void LineRenderer::render(const glm::vec3& p0, const glm::vec3& p1) {
    glm::vec3 resultVector = p0 - p1;
    glm::vec3 defaultVector(2.0f, 0.0f, 0.0f);

    glm::mat4 model, view, projection;

    projection = glm::perspective(45.0f, (GLfloat) 800 / (GLfloat) 600, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    view = glm::rotate(view, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // TODO - я не знаю, где я тут косячу, но по идее тут должен исходный вектор (2, 0, 0)
    // в нужный, но я где-то лажжаю
    glm::vec3 axis = glm::cross(defaultVector, resultVector);
    float angle = acos((float) glm::dot(p0, p1) / (p0 - p1).length());

    model = glm::rotate(model, angle, axis);
    float lineScaler = resultVector.length() / 2.0f;
    model = glm::scale(glm::vec3(lineScaler, lineScaler, lineScaler));
    glm::mat4 modelViewProjection = projection * view * model;

    lineShader.use();

//    Матрица точно подается в шейдер, т.к. я пробовал менять масштаб, линия реагирует
    GLuint mvpLocation = glGetUniformLocation(lineShader.getProgram(), "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(modelViewProjection));

    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

