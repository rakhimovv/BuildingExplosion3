//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_GAMESYSTEM_H
#define BUILDINGEXPLOSION3_GAMESYSTEM_H


#include "Block.h"
#include "../Utils/CachedArray.h"

class Block;

class GameSystem {
public:
    GameSystem(float constTimeStep);
    ~GameSystem();
    void Update(float dt);
    ParticleSystem<ParticleInfo> *GetParticleSystem();
private:
    CachedArray<Block *> blocks;
    ParticleSystem<ParticleInfo> *particleSystem;
    float constTimeStep;
};


#endif //BUILDINGEXPLOSION3_GAMESYSTEM_H
