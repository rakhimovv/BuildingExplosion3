//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_GAMESYSTEM_H
#define BUILDINGEXPLOSION3_GAMESYSTEM_H


#include "Block.h"
#include "../Utils/CachedArray.h"
#include "../CubeRenderer.h"
#include "../SkyboxRenderer.h"

class Block;

class GameSystem {
public:
    GameSystem(float constTimeStep);

    ~GameSystem();

    void Update(float dt);

    ParticleSystem<ParticleInfo> *GetParticleSystem();

    SkyBoxRenderer *GetSkyBoxRenderer();

    CubeRenderer *GetCubeRenderer();

private:
    CachedArray<Block *> blocks;
    ParticleSystem<ParticleInfo> *particleSystem;
    float constTimeStep;
    SkyBoxRenderer *skyBoxRenderer;
    CubeRenderer *cubeRenderer;
};


#endif //BUILDINGEXPLOSION3_GAMESYSTEM_H
