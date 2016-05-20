#include "GameShaders.h"

GameGraphic::GameGraphic(GameParameters& parameters)
{
    this->lineShader = new GameShader(parameters.GetLineVertexShader().c_str(), parameters.GetLineFragmentShader().c_str());
    this->cubeShader = new GameShader(parameters.GetCubeVertexShader().c_str(), parameters.GetCubeFragmentShader().c_str());
    this->skyboxShader = new GameShader(parameters.GetSkyboxVertexShader().c_str(), parameters.GetSkyboxFragmentShader().c_str());
}

GameGraphic::~GameGraphic()
{
    delete lineShader;
    delete cubeShader;
    delete skyboxShader;
}

GameShader * GameGraphic::GetLineShader()
{
    return this->lineShader;
}

GameShader * GameGraphic::GetCubeShader()
{
    return this->cubeShader;
}

GameShader * GameGraphic::GetSkyboxShader()
{
    return this->skyboxShader;
}

