//
// Created by aviator on 5/7/16.
//

#include "Camera.h"
#include "../WindowSize.h"

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(GameParameters& gameParameters): viewMatrix(), projectionMatrix(), cameraClock()
{
    cameraSpeed = gameParameters.GetCameraSpeed();
    cameraSpeedStep = gameParameters.GetCameraSpeedStep();

    yawAngle = gameParameters.GetYawAngle();
    pitchAngle = gameParameters.GetPitchAngle();

    yawAngleStep = glm::radians(80.0f);
    pitchAngleStep = glm::radians(80.0f);

    cameraPos = gameParameters.GetCameraPos();
//    glm::vec3 cameraTarget = glm::vec3(10.0f, 0.0f, -10.0f);

    cameraDirection = gameParameters.GetCameraDir();
//    cameraDirection = glm::normalize(cameraPos - cameraTarget);

    cameraUp = gameParameters.GetCameraUp();
    cameraRight = gameParameters.GetCameraRight();
    cameraFront = gameParameters.GetCameraFront();

    this->prevTime = 0;
    this->currentTime = 0;
    this->elapsedTime = 0;

    this->viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    this->projectionMatrix = glm::perspective(20.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
}

void Camera::MoveCamera(std::queue<sf::Keyboard::Key>& pressedButtons, std::queue<sf::Event::MouseMoveEvent> &mouseMoves) {
    glm::vec2 prevMousePos(0.0f, 0.0f);

    // Используем очередь
    while(!pressedButtons.empty()){
        ModifyCamera(pressedButtons.front());
        pressedButtons.pop();
    }

    while(!mouseMoves.empty()) {
        sf::Event::MouseMoveEvent mousePos = mouseMoves.front();

        float deltaX = mousePos.x - prevMousePos.x;
        float deltaY = mousePos.y - prevMousePos.y;

        yawAngle += deltaX * 0.001f;
        pitchAngle += deltaY * 0.001f;

        prevMousePos.x = mousePos.x;
        prevMousePos.y = mousePos.y;

        mouseMoves.pop();

        std::cout << "PitchAngle: <" << pitchAngle << ">" << std::endl;
        std::cout << "YawAngle: <" << yawAngle << ">" << std::endl;
    }
}

void Camera::ModifyCamera(sf::Keyboard::Key button)
{
    currentTime = cameraClock.getElapsedTime().asMilliseconds();
    elapsedTime = (currentTime - prevTime) * 0.01f;
    prevTime = currentTime;

    switch (button) {
        case sf::Keyboard::Key::W:
            cameraPos += cameraSpeed * cameraFront * elapsedTime;
            break;
        case sf::Keyboard::Key::S:
            cameraPos -= cameraSpeed * cameraFront * elapsedTime;
            break;
        case sf::Keyboard::Key::A:
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp)) * elapsedTime;
            break;
        case sf::Keyboard::Key::D:
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp)) * elapsedTime;
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
    std::cout << "cameraRight = (" << cameraRight.x << ", " << cameraRight.y << ", " << cameraRight.z << ")\n";
    std::cout << "cameraDir= (" << cameraDirection.x << ", " << cameraDirection.y << ", " << cameraDirection.z << ")\n";

    std::cout << "pitchAngle = " << pitchAngle << std::endl;
    std::cout << "yawAngle = " << yawAngle << std::endl;
}
