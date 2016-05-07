//
// Created by aviator on 5/7/16.
//
#pragma once

#include <queue>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <SFML/System/Clock.hpp>

enum  MoveDirection {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT
};

class Camera {
public:
    Camera();

    void MoveCamera(std::queue<sf::Keyboard::Key>& pressedButton);
    void PrintParameters();
    glm::mat4&  GetViewMatrix();
    glm::mat4&  GetProjectionMatrix();
private:
    void ModifyCamera(sf::Keyboard::Key button);
    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;

    glm::vec3 cameraFront;

    float cameraSpeed;
    float cameraSpeedStep;

    float pitchAngle;
    float pitchAngleStep;
    float yawAngle;
    float yawAngleStep;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    sf::Clock clock;
};