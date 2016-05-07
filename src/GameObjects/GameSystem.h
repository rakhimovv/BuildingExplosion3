//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_GAMESYSTEM_H
#define BUILDINGEXPLOSION3_GAMESYSTEM_H


#include "Block.h"
#include "../Utils/CachedArray.h"
#include "../CubeRenderer.h"
#include "../SkyboxRenderer.h"
#include "../LineRenderer.h"
#include "Explosion.h"
#include "Bomb.h"

class Block;

class Bomb;

class Explosion;

class GameSystem {
public:
    GameSystem(float constTimeStep);

    ~GameSystem();

    void Update(float dt);

    ParticleSystem<ParticleInfo> *GetParticleSystem();

    void SetExplosion(Explosion *explosion);

    SkyBoxRenderer *GetSkyBoxRenderer();

    CubeRenderer *GetCubeRenderer();

private:
    CachedArray<Block *> blocks;
    ParticleSystem<ParticleInfo> *particleSystem;
    float constTimeStep;
    SkyBoxRenderer *skyBoxRenderer;
    CubeRenderer *cubeRenderer;
    LineRenderer *lineRenderer;
    Explosion *explosion;
    Bomb *bomb;
};


#endif //BUILDINGEXPLOSION3_GAMESYSTEM_H
