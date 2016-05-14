#ifndef GAMESHADERS_H
#define GAMESHADERS_H

#include "shader.h"
#include "GameParameters.h"

class GameGraphic
{
public:
    GameGraphic(GameParameters& gameParameters);
    ~GameGraphic();

    GameShader * GetLineShader();
    GameShader * GetCubeShader();
    GameShader * GetSkyboxShader();

private:
    GameShader * lineShader;
    GameShader * cubeShader;
    GameShader * skyboxShader;
};

#endif // GAMESHADERS_H
