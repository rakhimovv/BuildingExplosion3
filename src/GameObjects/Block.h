//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_BLOCK_H
#define BUILDINGEXPLOSION3_BLOCK_H

#include <iostream>
#include <vector>
#include "../Maths/Vector3f.h"
#include "../Physics/ParticleSystem.h"
#include "../Physics/ParticleInfo.h"
#include "GameSystem.h"

class GameSystem;

class Block {
public:
    struct Descriptor {
        Descriptor();

        std::vector<Vector3f> vertexPositions;
    };

    Block(const Descriptor &desc, GameSystem *owner);

    ~Block();

    void Update(float dt);

    void Render();

    bool Exists();

private:
    GameSystem *owner;
    std::vector<ParticleHandle<ParticleInfo>> particleHandles;
    bool exists;
};


#endif //BUILDINGEXPLOSION3_BLOCK_H
