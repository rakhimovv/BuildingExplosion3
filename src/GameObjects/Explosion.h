//
// Created by Руслан Рахимов on 06/05/16.
//

#ifndef BUILDINGEXPLOSION3_EXPLOSION_H
#define BUILDINGEXPLOSION3_EXPLOSION_H


#include "../Maths/Vector3f.h"
#include "GameSystem.h"

class GameSystem;

class Explosion {
public:
    struct Descriptor {
        Descriptor();

        float maxRadius;
        Vector3f pos;
        float totalLifetime;
    };

    Explosion(const Descriptor &desc, GameSystem *owner);

    ~Explosion();

    void Update(float dt);

    void Render();

    bool Exists();

private:
    GameSystem *owner;
    float maxRadius;
    Vector3f pos;
    float totalLifetime;
    float elapsedLifetime;
    bool exists;
};


#endif //BUILDINGEXPLOSION3_EXPLOSION_H
