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
#include "Camera.h"

class GameSystem;

class Camera;

class Block {
public:
    struct Descriptor {
        Descriptor();

        std::vector<Vector3f> vertexPositions;
        float edgeLength;
        bool fixed = false;
    };

    Block(const Descriptor &desc, GameSystem *owner);

    ~Block();

    void Update(float dt);

    ParticleHandle<ParticleInfo> *GetParticleHandle(int id);

    void Render();

    bool Exists();

private:
    GameSystem *owner;
    Camera * camera;
    std::vector<ParticleHandle<ParticleInfo>> particleHandles;
    float edgeLength;
    bool exists;
};


#endif //BUILDINGEXPLOSION3_BLOCK_H
