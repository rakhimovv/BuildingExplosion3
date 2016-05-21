//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_BOMB_H
#define BUILDINGEXPLOSION3_BOMB_H


#include "GameSystem.h"
#include "Camera.h"

class Bomb {
public:
    bool boom = false;

    struct Descriptor {
        Descriptor();
        Vector3f pos;
        float edgeLength;
    };

    Bomb(const Descriptor &desc, GameSystem *owner);

    ~Bomb();

    void Boom(float dt);

    void Render();

    bool Exists();

private:
    GameSystem *owner;
    float edgeLength;
    ParticleHandle<ParticleInfo> particleHandle;
    bool exists;
};


#endif //BUILDINGEXPLOSION3_BOMB_H
