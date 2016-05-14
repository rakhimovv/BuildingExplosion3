//
// Created by aviator on 5/11/16.
//

#ifndef BUILDINGEXPLOSION3_LINE_H
#define BUILDINGEXPLOSION3_LINE_H

#include "Mesh.h"
#include "../shader.h"
#include "../GameParameters.h"
#include "Camera.h"

#include <SFML/Window.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Line : public Mesh {
public:
    Line(glm::vec3& p0, glm::vec3& p1, glm::vec3& color, GameShader * lineShader);

    //! @overload
    virtual std::vector<float>& getVertices();
    //! @overload
    virtual std::vector<int>& getIndexes();


    void render(Camera& camera);

    void dump();

    void update(glm::vec3 * newPointOne, glm::vec3 * newPointTwo);
    void updateColor(glm::vec3 * newColor);
    void updateFirstPoint(glm::vec3 * newPointOne);
    void updateSecondPoint(glm::vec3 * newPointTwo);

private:
    glm::vec3 pointOne;
    glm::vec3 pointTwo;
    glm::vec3 lineColor;

    std::vector<float> lineVertices;
    std::vector<int> lineIndexes;

    GLuint vertexBufferObject;
    GLuint vertexArrayObject;

    GameShader * lineShader;

//    static GameShader lineShader(gameParameters.GetLineVertexShader().c_str(), gameParameters.GetLineFragmentShader().c_str());
};


#endif //BUILDINGEXPLOSION3_LINE_H
