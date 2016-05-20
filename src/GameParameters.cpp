//
// Created by aviator on 5/10/16.
//

#define DEBUG
#define NO_INDEX -1

#include "GameParameters.h"
#include <fstream>
#include <stdexcept>
#include <assert.h>

GameParameters::GameParameters(const char * configFilePath) {
    std::fstream input(configFilePath);

    if (!input.is_open()) {
        throw std::invalid_argument("Unable to open config file: <" + std::string(configFilePath) + ">\n");
    }

    Json::Reader jsonReader;
    Json::Value configRoot;

    if (!jsonReader.parse(input, configRoot)) {
        throw std::invalid_argument("Json parsing failed: " + jsonReader.getFormattedErrorMessages() + "\n");
    } else {
        std::cout << "Json compilation is successful!" << std::endl;
    }

    // Load shaders path
    loadStringParameter(configRoot, "shaders", "cubeFragment", cubeFragmentShaderPath);
    loadStringParameter(configRoot, "shaders", "cubeVertex", cubeVertexShaderPath);
    loadStringParameter(configRoot, "shaders", "lineFragment", lineFragmentShaderPath);
    loadStringParameter(configRoot, "shaders", "lineVertex", lineVertexShaderPath);
    loadStringParameter(configRoot, "shaders", "skyboxFragment", skyboxFragmentShaderPath);
    loadStringParameter(configRoot, "shaders", "skyboxVertex", skyboxVertexShaderPath);
    loadStringParameter(configRoot, "shaders", "sphereVertex", sphereVertexShaderPath);
    loadStringParameter(configRoot, "shaders", "sphereFragment", sphereFragmentShaderPath);

    // Load textures paths
    loadStringParameter(configRoot, "textures", "cubeTexture", cubeTexturePath);

    skyboxTexturePaths.resize(10);
    loadStringParameter(configRoot, "textures", "skyboxRightTexture", skyboxTexturePaths[0]);
    loadStringParameter(configRoot, "textures", "skyboxLeftTexture", skyboxTexturePaths[1]);
    loadStringParameter(configRoot, "textures", "skyboxUpTexture", skyboxTexturePaths[2]);
    loadStringParameter(configRoot, "textures", "skyboxBottomTexture", skyboxTexturePaths[3]);
    loadStringParameter(configRoot, "textures", "skyboxBackTexture", skyboxTexturePaths[4]);
    loadStringParameter(configRoot, "textures", "skyboxFrontTexture", skyboxTexturePaths[5]);

    // Load camera parameters
    loadFloatParameter(configRoot, "camera", "cameraSpeed", cameraSpeed);
    loadFloatParameter(configRoot, "camera", "cameraSpeedStep", cameraSpeedStep);

    loadFloatParameter(configRoot, "camera", "cameraPos_x", cameraPos.x);
    loadFloatParameter(configRoot, "camera", "cameraPos_y", cameraPos.y);
    loadFloatParameter(configRoot, "camera", "cameraPos_z", cameraPos.z);

    loadFloatParameter(configRoot, "camera", "cameraFront_x", cameraFront.x);
    loadFloatParameter(configRoot, "camera", "cameraFront_y", cameraFront.y);
    loadFloatParameter(configRoot, "camera", "cameraFront_z", cameraFront.z);

    loadFloatParameter(configRoot, "camera", "cameraUp_x", cameraUp.x);
    loadFloatParameter(configRoot, "camera", "cameraUp_y", cameraUp.y);
    loadFloatParameter(configRoot, "camera", "cameraUp_z", cameraUp.z);

    loadFloatParameter(configRoot, "camera", "cameraRight_x", cameraRight.x);
    loadFloatParameter(configRoot, "camera", "cameraRight_y", cameraRight.y);
    loadFloatParameter(configRoot, "camera", "cameraRight_z", cameraRight.z);

    loadFloatParameter(configRoot, "camera", "cameraDir_x", cameraDirection.x);
    loadFloatParameter(configRoot, "camera", "cameraDir_y", cameraDirection.y);
    loadFloatParameter(configRoot, "camera", "cameraDir_z", cameraDirection.z);

    loadFloatParameter(configRoot, "camera", "pitchAngle", pitchAngle);
    loadFloatParameter(configRoot, "camera", "yawAngle", yawAngle);

    dumpParameters();
}

void GameParameters::loadStringParameter(const Json::Value& configRoot, const char * parameterType, const char * parameterName,
    std::string& stringValue)
{
    assert(parameterType);
    assert(parameterName);

    Json::Value parameterValue = configRoot[parameterType][parameterName];

    if (parameterValue.isNull()) {
        throw std::invalid_argument("JSON::[" + std::string(parameterType) + "][" + std::string(parameterName) + "]");
    } else {
        std::cout << "JSON:: loaded [" << std::string(parameterType) << "][" << std::string(parameterName) << "]" <<
        std::endl;
    }

    stringValue = parameterValue.asString();
}

void GameParameters::loadFloatParameter(const Json::Value& configRoot, const char * parameterType, const char * parameterName,
                                         float &floatValue)
{
    assert(parameterType);
    assert(parameterName);

    Json::Value parameterValue = configRoot[parameterType][parameterName];

//    if (index == NO_INDEX) {
//        parameterValue = configRoot[parameterType][parameterName];
//    } else {
//        parameterValue = configRoot[parameterType][parameterName][index];
//    }

    if (parameterValue.isNull()) {
        throw std::invalid_argument("JSON::[" + std::string(parameterType) + "][" + std::string(parameterName) + "]");
    } else {
        std::cout << "JSON:: loaded [" << std::string(parameterType) << "][" << std::string(parameterName) << "]" <<
        std::endl;
    }

    floatValue = parameterValue.asFloat();
}

void GameParameters::dumpParameters()
{
    std::cout << std::endl;

    std::cout << "cubeFragmentShaderPath: <" << cubeFragmentShaderPath << ">" << std::endl;
    std::cout << "cubeVertexShaderPath: <" << cubeVertexShaderPath << ">" << std::endl;
    std::cout << "lineFragmentShaderPath: <" << lineFragmentShaderPath << ">" << std::endl;
    std::cout << "lineVertexShaderPath: <" << lineVertexShaderPath << ">" << std::endl;
    std::cout << "skyboxFragmentShaderPath: <" << skyboxFragmentShaderPath << ">" << std::endl;
    std::cout << "skyboxVertexShaderPath: <" << skyboxVertexShaderPath << ">" << std::endl;
    std::cout << "sphereVertexShaderPath: <" << sphereVertexShaderPath << ">" << std::endl;
    std::cout << "sphereFragmentShaderPath: <" << sphereFragmentShaderPath << ">" << std::endl;

    std::cout << "cubeTexturePath: <" << cubeTexturePath << ">" << std::endl;

    std::cout << "cameraSpeed: <" << cameraSpeed << ">" << std::endl;
    std::cout << "cameraSpeedStep: <" << cameraSpeedStep << ">" << std::endl;

    std::cout << "cameraPos.x <" << cameraPos.x << ">" << std::endl;
    std::cout << "cameraPos.y <" << cameraPos.y << ">" << std::endl;
    std::cout << "cameraPos.z <" << cameraPos.z << ">" << std::endl;

    std::cout << "cameraFront.x <" << cameraFront.x << ">" << std::endl;
    std::cout << "cameraFront.y <" << cameraFront.y << ">" << std::endl;
    std::cout << "cameraFront.z <" << cameraFront.z << ">" << std::endl;

    std::cout << "cameraUp.x <" << cameraUp.x << ">" << std::endl;
    std::cout << "cameraUp.y <" << cameraUp.y << ">" << std::endl;
    std::cout << "cameraUp.z <" << cameraUp.z << ">" << std::endl;

    std::cout << "cameraRight.x <" << cameraRight.x << ">" << std::endl;
    std::cout << "cameraRight.y <" << cameraRight.y << ">" << std::endl;
    std::cout << "cameraRight.z <" << cameraRight.z << ">" << std::endl;

    std::cout << "cameraDir.x <" << cameraDirection.x << ">" << std::endl;
    std::cout << "cameraDir.y <" << cameraDirection.y << ">" << std::endl;
    std::cout << "cameraDir.z <" << cameraDirection.z << ">" << std::endl;

    std::cout << std::endl;
}

const std::string& GameParameters::GetCubeFragmentShader()
{
    return this->cubeFragmentShaderPath;
}

const std::string& GameParameters::GetCubeVertexShader()
{
    return this->cubeVertexShaderPath;
}

const std::string& GameParameters::GetLineVertexShader()
{
    return this->lineVertexShaderPath;
}

const std::string& GameParameters::GetLineFragmentShader()
{
    return this->lineFragmentShaderPath;
}

const std::string& GameParameters::GetSkyboxFragmentShader()
{
    return this->skyboxFragmentShaderPath;
}

const std::string& GameParameters::GetSkyboxVertexShader()
{
    return this->skyboxVertexShaderPath;
}

const std::string& GameParameters::GetSphereVertexShader()
{
    return this->sphereVertexShaderPath;
}

const std::string& GameParameters::GetSphereFragmentShader()
{
    return this->sphereFragmentShaderPath;
}

const std::string& GameParameters::GetCubeTexture()
{
    return this->cubeTexturePath;
}

const std::vector<std::string>& GameParameters::GetSkyboxTextures()
{
    return this->skyboxTexturePaths;
}

float GameParameters::GetCameraSpeed()
{
    return this->cameraSpeed;
}

float GameParameters::GetCameraSpeedStep()
{
    return this->cameraSpeedStep;
}

float GameParameters::GetPitchAngle()
{
    return this->pitchAngle;
}

float GameParameters::GetYawAngle()
{
    return this->yawAngle;
}

glm::vec3 GameParameters::GetCameraPos()
{
    return this->cameraPos;
}

glm::vec3 GameParameters::GetCameraFront()
{
    return this->cameraFront;
}

glm::vec3 GameParameters::GetCameraUp()
{
    return this->cameraUp;
}

glm::vec3 GameParameters::GetCameraRight()
{
    return this->cameraRight;
}

glm::vec3 GameParameters::GetCameraDir()
{
    return this->cameraDirection;
}
