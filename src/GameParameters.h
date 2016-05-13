//
// Created by aviator on 5/10/16.
//

#ifndef BUILDINGEXPLOSION3_GAMEPARAMETERS_H
#define BUILDINGEXPLOSION3_GAMEPARAMETERS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "../dependencies/json/json.h"

class GameParameters {
public:
    GameParameters(const char * configFilePath);

    const std::string& GetCubeFragmentShader();
    const std::string& GetCubeVertexShader();
    const std::string& GetLineVertexShader();
    const std::string& GetLineFragmentShader();
    const std::string& GetSkyboxFragmentShader();
    const std::string& GetSkyboxVertexShader();
    const std::string& GetSphereVertexShader();
    const std::string& GetSphereFragmentShader();

    const std::string& GetCubeTexture();
    const std::vector<std::string>& GetSkyboxTextures();

    float GetCameraSpeed();
    float GetCameraSpeedStep();
private:
    // Shader paths
    std::string cubeFragmentShaderPath;
    std::string cubeVertexShaderPath;
    std::string lineFragmentShaderPath;
    std::string lineVertexShaderPath;
    std::string skyboxFragmentShaderPath;
    std::string skyboxVertexShaderPath;
    std::string sphereVertexShaderPath;
    std::string sphereFragmentShaderPath;

    //Texture options
    std::vector<std::string> skyboxTexturePaths;
    std::string cubeTexturePath;
    //Camera options
    float cameraSpeed;
    float cameraSpeedStep;

    void loadStringParameter(const Json::Value& configRoot, const char * parameterType, const char * parameterName, std::string& stringValue);
    void loadFloatParameter(const Json::Value& configRoot, const char * parameterType, const char * parameterName, float & floatValue);

    void dumpParameters();
};


#endif //BUILDINGEXPLOSION3_GAMEPARAMETERS_H
