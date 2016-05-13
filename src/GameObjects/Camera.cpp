//
// Created by aviator on 5/7/16.
//

#include "Camera.h"
#include "../WindowSize.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(GameParameters& gameParameters): clock(), viewMatrix(), projectionMatrix()
{
    cameraSpeed = gameParameters.GetCameraSpeed();
    cameraSpeedStep = gameParameters.GetCameraSpeedStep();

    yawAngle = 1.0f;
    pitchAngle = 1.0f;

    yawAngleStep = glm::radians(80.0f);
    pitchAngleStep = glm::radians(80.0f);

    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(10.0f, 0.0f, -10.0f);

    cameraDirection = glm::normalize(cameraPos - cameraTarget);

    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
    cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);

    this->viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    this->projectionMatrix = glm::perspective(20.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
}

void Camera::MoveCamera(std::queue<sf::Keyboard::Key>& pressedButtons) {
//    std::cout << "Camera moved" << std::endl;

    // Используем очередь
    while(!pressedButtons.empty()){
//         1 нажатие клавиши
        ModifyCamera(pressedButtons.front());
        pressedButtons.pop();
    }
}

void Camera::ModifyCamera(sf::Keyboard::Key button)
{
    switch (button) {
        case sf::Keyboard::Key::W:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case sf::Keyboard::Key::S:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case sf::Keyboard::Key::A:
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        case sf::Keyboard::Key::D:
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        case sf::Keyboard::Key::Add:
            cameraSpeed += cameraSpeedStep;
            break;
        case sf::Keyboard::Key::Subtract:
            cameraSpeed -= cameraSpeedStep;
            break;
        case sf::Keyboard::Key::Up:
            pitchAngle += pitchAngleStep;
            break;
        case sf::Keyboard::Key::Down:
            pitchAngle -= pitchAngleStep;
            break;
        case sf::Keyboard::Key::Left:
            yawAngle -= yawAngleStep;
            break;
        case sf::Keyboard::Key::Right:
            yawAngle += yawAngleStep;
            break;
        default:
            break;
    }

    cameraFront.x = cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
    cameraFront.y = sin(glm::radians(pitchAngle));
    cameraFront.z = sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));

    cameraFront = glm::normalize(cameraFront);


    if(pitchAngle > 89.0f)
        pitchAngle = 89.0f;
    if(pitchAngle < -89.0f)
        pitchAngle = -89.0f;
}

const glm::mat4&  Camera::GetViewMatrix() {
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() {
    return projectionMatrix;
}

void Camera::PrintParameters()
{
    std::cout << "cameraPos = (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
    std::cout << "cameraFront = (" << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << ")\n";
    std::cout << "cameraUp = (" << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << ")\n";
    std::cout << "cameraDir= (" << cameraDirection.x << ", " << cameraDirection.y << ", " << cameraDirection.z << ")\n";
}
