#include "Sphere.h"
#include <glm/gtc/type_ptr.hpp>

Sphere::Sphere(glm::vec3 * centerCoord, float radius, glm::vec3 * color, GameParameters& gameParameters):
    sphereShader(gameParameters.GetSphereVertexShader().c_str(), gameParameters.GetSphereFragmentShader().c_str())
{
    this->sphereCenter.x = centerCoord->x;
    this->sphereCenter.y = centerCoord->y;
    this->sphereCenter.z = centerCoord->z;

    this->radius = radius;

    this->sphereColor.x = color->x;
    this->sphereColor.y = color->y;
    this->sphereColor.z = color->z;

    int lats = 60;
    int longs = 60;

    int indicator = 0;

    for(int i = 0; i <= lats; i++) {
        float lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / lats);
        float z0  = sin(lat0);
        float zr0 =  cos(lat0);

        float lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for(int j = 0; j <= longs; j++) {
            float lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            sphereVertices.push_back(x * zr0);
            sphereVertices.push_back(y * zr0);
            sphereVertices.push_back(z0);
            sphereIndices.push_back(indicator);
            indicator++;

            sphereVertices.push_back(x * zr1);
            sphereVertices.push_back(y * zr1);
            sphereVertices.push_back(z1);
            sphereIndices.push_back(indicator);
            indicator++;
        }

        sphereIndices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), &sphereVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLfloat), &sphereIndices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(GLfloat), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    this->indicesToDraw = sphereIndices.size();
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
}

std::vector<float>& Sphere::getVertices()
{
    return this->sphereVertices;
}

std::vector<int>& Sphere::getIndexes()
{
    return this->sphereIndices;
}

void Sphere::render(Camera &camera)
{
    sphereShader.use();

    glm::mat4 modelMatrix;
//    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f / radius, 0.5f / radius, 0.5f / radius));

//    this->viewMatrix = camera.GetViewMatrix();
    this->projectionMatrix = camera.GetProjectionMatrix();

    glm::mat4 modelViewProjection = projectionMatrix * viewMatrix * modelMatrix;

    GLuint mvpLocation = glGetUniformLocation(sphereShader.getProgram(), "MVP");
    GLuint colorLocation = glGetUniformLocation(sphereShader.getProgram(), "sphereColor");

    glBindVertexArray(vertexArrayObject);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObject);

    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(modelViewProjection));
    glUniform3f(colorLocation, sphereColor.x, sphereColor.y, sphereColor.z);

    glDrawElements(GL_QUAD_STRIP, indicesToDraw, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}

void Sphere::setCenter(glm::vec3 * newCenter)
{
    this->sphereCenter.x = newCenter->x;
    this->sphereCenter.y = newCenter->y;
    this->sphereCenter.z = newCenter->z;
}

//#include "Sphere.h"

//#include <vector>
//#include <iostream>
//#include <glm/gtc/matrix_inverse.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>

//Sphere::Sphere()
//{
//    isInited = false;
//    m_vao = 0;
//    m_vboVertex = 0;
//    m_vboIndex = 0;

//    lats = 40;
//    longs = 40;
//}

//Sphere::~Sphere()
//{

//}

//void Sphere::init(GLuint vertexPositionID)
//{
//    int i, j;

//    std::vector<GLfloat> vertices;
//    std::vector<GLuint> indices;
//    int indicator = 0;

//    for(i = 0; i <= lats; i++) {
//        double lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / lats);
//       double z0  = sin(lat0);
//       double zr0 =  cos(lat0);

//       double lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
//       double z1 = sin(lat1);
//       double zr1 = cos(lat1);

//       for(j = 0; j <= longs; j++) {
//           double lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
//           double x = cos(lng);
//           double y = sin(lng);

//           vertices.push_back(x * zr0);
//           vertices.push_back(y * zr0);
//           vertices.push_back(z0);
//           indices.push_back(indicator);
//           indicator++;

//           vertices.push_back(x * zr1);
//           vertices.push_back(y * zr1);
//           vertices.push_back(z1);
//           indices.push_back(indicator);
//           indicator++;
//       }

//       indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
//   }

//    // 创建并绑定环境
//    glGenVertexArrays(1, &m_vao);
//    glBindVertexArray(m_vao);

//    glGenBuffers(1, &m_vboVertex);
//    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

//    glVertexAttribPointer(vertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//    glEnableVertexAttribArray (vertexPositionID);

//    glGenBuffers(1, &m_vboIndex);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

//    numsToDraw = indices.size();

//    isInited = true;
//}

//void Sphere::cleanup()
//{
//    if (!isInited) {
//        return;
//    }
//    if(m_vboVertex) {
//        glDeleteBuffers(1, &m_vboVertex);
//    }
//    if(m_vboIndex) {
//        glDeleteBuffers(1, &m_vboIndex);
//    }
//    if (m_vao) {
//        glDeleteVertexArrays(1, &m_vao);
//    }

//    isInited = false;
//    m_vao = 0;
//    m_vboVertex = 0;
//    m_vboIndex = 0;
//}

//void Sphere::draw()
//{
//    if (!isInited) {
//        std::cout << "please call init() before draw()" << std::endl;
//    }

//    // draw sphere
//    glBindVertexArray(m_vao);
//    glEnable(GL_PRIMITIVE_RESTART);
//    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
//    glDrawElements(GL_QUAD_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
//}
