//
// Created by aviator on 5/11/16.
//

#include "Line.h"
#include "Camera.h"
#include "../GameParameters.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Line::Line(glm::vec3& p0, glm::vec3& p1, glm::vec3& color, GameShader * shader):
    pointOne(p0.x, p0.y, p0.z), pointTwo(p1.x, p1.y, p1.z), lineColor(color.x, color.y, color.z),
    lineShader(shader)
{
    assert(this->lineShader);

    // First point vertices
    lineVertices.push_back(p0.x);
    lineVertices.push_back(p0.y);
    lineVertices.push_back(p0.z);
    // Second point vertices
    lineVertices.push_back(p1.x);
    lineVertices.push_back(p1.y);
    lineVertices.push_back(p1.z);

    // Сейчас отрисовка только по вертексам, отрисовку по индексам добавлю позже
    // TODO (Булат): добавить корректные индексы
    lineIndexes.push_back(1);
    lineIndexes.push_back(2);

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(GLfloat), &lineVertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}


std::vector<float>& Line::getVertices()
{
    return this->lineVertices;
}

std::vector<int>& Line::getIndexes()
{
    return this->lineIndexes;
}

void Line::render(Camera& camera)
{
    // Загружаем обновленные вертексы в наш буфер
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(GLfloat), &lineVertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Считаем матрицы координатных преобразований
    glm::mat4 model;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    glm::mat4 modelViewProjection = projection * view * model;

    lineShader->use();

    GLuint mvpLocation = glGetUniformLocation(lineShader->getProgram(), "MVP");
    GLuint colorLocation = glGetUniformLocation(lineShader->getProgram(), "ourColor");

    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(modelViewProjection));
    glUniform3f(colorLocation, lineColor.x, lineColor.y, lineColor.z);

    // Рендерим
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Line::dump()
{
    std::cout << "DEBUG::LINE:: vertices: " << lineVertices.size() << std::endl;
    for (int i = 0; i < lineVertices.size(); i++) {
        std::cout << "[" << lineVertices[i] << "]" << std::endl;
    }
}

void Line::update(glm::vec3 * newPointOne, glm::vec3 * newPointTwo)
{
    assert(newPointOne);
    assert(newPointTwo);

    pointOne.x = newPointOne->x;
    pointOne.y = newPointOne->y;
    pointOne.z = newPointOne->z;

    lineVertices[0] = newPointOne->x;
    lineVertices[1] = newPointOne->y;
    lineVertices[2] = newPointOne->z;

    lineVertices[3] = newPointTwo->x;
    lineVertices[4] = newPointTwo->y;
    lineVertices[5] = newPointTwo->z;
}

void Line::updateFirstPoint(glm::vec3 * newPointOne)
{
    pointOne.x = newPointOne->x;
    pointOne.y = newPointOne->y;
    pointOne.z = newPointOne->z;

    lineVertices[0] = newPointOne->x;
    lineVertices[1] = newPointOne->y;
    lineVertices[2] = newPointOne->z;
}

void Line::updateSecondPoint(glm::vec3 * newPointTwo)
{
    pointTwo.x = newPointTwo->x;
    pointTwo.y = newPointTwo->y;
    pointTwo.z = newPointTwo->z;

    lineVertices[3] = newPointTwo->x;
    lineVertices[4] = newPointTwo->y;
    lineVertices[5] = newPointTwo->z;
}

void Line::updateColor(glm::vec3 * newColor)
{
    assert(newColor);

    lineColor.x = newColor->x;
    lineColor.y = newColor->y;
    lineColor.z = newColor->z;
}
