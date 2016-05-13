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
#include "../LineRenderer.h"
#include "Explosion.h"
#include "Bomb.h"
#include "../Physics/AosParticleSystem.h"

class Block;

class Bomb;
class Explosion;

class Camera;

class GameSystem {
public:
    GameSystem(float constTimeStep);

    ~GameSystem();

    void Update(float dt, std::queue<sf::Keyboard::Key>& pressedButtons);

    ParticleSystem<ParticleInfo> *GetParticleSystem();

    void SetExplosion(Explosion *explosion);

    SkyBoxRenderer *GetSkyBoxRenderer();

    CubeRenderer *GetCubeRenderer();

    Camera * GetCamera();

private:
    CachedArray<Block *> blocks;
    AosParticleSystem<ParticleInfo> *particleSystem;
    float constTimeStep;
    SkyBoxRenderer *skyBoxRenderer;
    CubeRenderer *cubeRenderer;
    LineRenderer *lineRenderer;
    Explosion *explosion;
    Camera * camera;
    Bomb *bomb;
};


#endif //BUILDINGEXPLOSION3_GAMESYSTEM_H
