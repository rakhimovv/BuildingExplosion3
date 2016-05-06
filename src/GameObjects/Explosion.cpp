//
// Created by Руслан Рахимов on 06/05/16.
//

#include "Explosion.h"

Explosion::Descriptor::Descriptor() { }

Explosion::Explosion(const Descriptor &desc, GameSystem *owner) {
    this->exists = true;
    this->owner = owner;

    this->maxRadius = desc.maxRadius;
    this->pos = desc.pos;
    this->totalLifetime = desc.totalLifetime;
    this->elapsedLifetime = desc.totalLifetime;
}

Explosion::~Explosion() { }

void Explosion::Update(float dt) {
    this->elapsedLifetime -= dt;
    if (this->elapsedLifetime < 0.0f) {
        this->exists = false;
    }

    // Обработка взрыва
    float currRadius = ((totalLifetime - elapsedLifetime) / totalLifetime) * this->maxRadius;

    for (int i = 0; i < owner->GetParticleSystem()->GetParticlesCount(); i++) {
        ParticleHandle<ParticleInfo> blockParticle = owner->GetParticleSystem()->GetParticle(size_t(i));

        Vector3f distance = blockParticle.GetPos() - this->pos;
        if (distance.Length() <= currRadius) {
            float offset = 0.3f;
            Vector3f resultAcceleration = blockParticle.GetAcceleration() + offset * distance.GetNorm();
            blockParticle.SetAcceleration(resultAcceleration);
        }
    }

    Explosion::Render();
}

void Explosion::Render() {
}

bool Explosion::Exists() {
    return this->exists;
}