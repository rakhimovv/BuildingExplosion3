//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_GAMESYSTEM_H
#define BUILDINGEXPLOSION3_GAMESYSTEM_H

#include <queue>
#include <SFML/Window.hpp>
#include "Block.h"
#include "../Utils/CachedArray.h"
#include "../CubeRenderer.h"
#include "../SkyboxRenderer.h"
#include "../GameParameters.h"
#include "Explosion.h"
#include "Bomb.h"
#include "Line.h"
#include "../Physics/AosParticleSystem.h"
#include "../GameShaders.h"


class Block;

class Bomb;
class Explosion;

class Camera;

/*
class LinkLine {

    struct Descriptor {
        size_t p0;
        size_t p1;
        auto line;
    };

    LinkLine(const Descriptor &desc);

    ~LinkLine();

    size_t p0;
    size_t p1;
    Line line;
};
 */

class GameSystem {
public:
    struct LinkLine {
        size_t p0;
        size_t p1;
        Line * line;
    };

    GameSystem(float constTimeStep);

    ~GameSystem();

    void Update(float dt, std::queue<sf::Keyboard::Key>& pressedButtons, std::queue<sf::Event::MouseMoveEvent>& mouseMoves);

    ParticleSystem<ParticleInfo> *GetParticleSystem();

    void SetExplosion(Explosion *explosion);

    SkyBoxRenderer *GetSkyBoxRenderer();

    CubeRenderer *GetCubeRenderer();

    Camera * GetCamera();

private:
    CachedArray<LinkLine> linkLine;
    //CachedArray<LinkLine *> linkLine;
    CachedArray<Block *> blocks;
    AosParticleSystem<ParticleInfo> * particleSystem;
    float constTimeStep;
    GameGraphic * gameGraphic;
    SkyBoxRenderer * skyBoxRenderer;
    CubeRenderer * cubeRenderer;
    GameParameters gameParameters;
    Explosion * explosion;
    Camera * camera;
    Bomb * bomb;
};


#endif //BUILDINGEXPLOSION3_GAMESYSTEM_H
