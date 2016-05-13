//
// Created by aviator on 5/10/16.
//

#define DEBUG

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

    // Load textures paths
    loadStringParameter(configRoot, "textures", "cubeTexture", cubeTexturePath);
//    loadParameter(configRoot, "textures", "skyboxTexture", cubeTexturePath);

    // Load camera parameters
    loadFloatParameter(configRoot, "camera", "cameraSpeed", cameraSpeed);
    loadFloatParameter(configRoot, "camera", "cameraSpeedStep", cameraSpeedStep);

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
    std::cout << "cubeFragmentShaderPath: <" << cubeFragmentShaderPath << ">" << std::endl;
    std::cout << "cubeVertexShaderPath: <" << cubeVertexShaderPath << ">" << std::endl;
    std::cout << "lineFragmentShaderPath: <" << lineFragmentShaderPath << ">" << std::endl;
    std::cout << "lineVertexShaderPath: <" << lineVertexShaderPath << ">" << std::endl;
    std::cout << "skyboxFragmentShaderPath: <" << skyboxFragmentShaderPath << ">" << std::endl;
    std::cout << "skyboxVertexShaderPath: <" << skyboxVertexShaderPath << ">" << std::endl;

    std::cout << "cubeTexturePath: <" << cubeTexturePath << ">" << std::endl;

    std::cout << "cameraSpeed: <" << cameraSpeed << ">" << std::endl;
    std::cout << "cameraSpeedStep: <" << cameraSpeedStep << ">" << std::endl;
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