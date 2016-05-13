#pragma once

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "../GameParameters.h"
#include "../shader.h"

class Sphere : public Mesh
{
public:
    Sphere(glm::vec3 * centerCoord, float radius, glm::vec3 * color, GameParameters& gameParameters);
    ~Sphere();

    void render(Camera &camera);

    void setCenter(glm::vec3 * newCenter);
    void setRadius(float newRadius);

    //! @overload
    virtual std::vector<float>& getVertices();
    //! @overload
    virtual std::vector<int>& getIndexes();
private:
    glm::vec3 sphereCenter;
    float radius;

    glm::vec3 sphereColor;

    std::vector<float> sphereVertices;
    std::vector<int> sphereIndices;

    GLuint indicesToDraw;

    GameShader sphereShader;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    GLuint vertexArrayObject;
    GLuint vertexBufferObject;
    GLuint elementBufferObject;
};

//#ifndef SPHERE_H
//#define SPHERE_H

//#include <GL/glew.h>

//class Sphere
//{
//public:
//    Sphere();
//    ~Sphere();
//    void init(GLuint vertexPositionID);
//    void cleanup();
//    void draw();

//private:
//    int lats, longs;
//    bool isInited;
//    GLuint m_vao, m_vboVertex, m_vboIndex;
//    int numsToDraw;
//};

//#endif // SPHERE_H
