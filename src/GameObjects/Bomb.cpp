//
// Created by Руслан Рахимов on 06/05/16.
//

#include "Bomb.h"

Bomb::Descriptor::Descriptor() {
    this->pos = Vector3f(0.0f, 0.0f, 0.0f);
}

Bomb::Bomb(const Descriptor &desc, GameSystem *owner) {
    this->exists = true;
    this->owner = owner;
    this->edgeLength = desc.edgeLength;
    this->particleHandle = owner->GetParticleSystem()->AddParticle(desc.pos, desc.edgeLength / 2.0f, false);
    this->force = desc.force;
}

Bomb::~Bomb() {
    particleHandle.Release();
}

void Bomb::Boom(float dt) {
    // TODO замутить движение бомбы или привязать к камере ее

    if (boom) {//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        this->exists = false;
        // ВЗРЫВ
        //std::cout << "ВЗРЫВ!\n";

        Explosion::Descriptor explosionDesc;
        explosionDesc.maxRadius = 1.0f;
        explosionDesc.totalLifetime = 2.0f;
        explosionDesc.pos = particleHandle.GetPos();
        explosionDesc.force = force;

        owner->SetExplosion(new Explosion(explosionDesc, owner));
    }
}

void Bomb::Render() {
    // TODO
    Vector3f pos = particleHandle.GetPos();
    Cube cube(glm::vec3(pos.x, pos.y, pos.z), edgeLength); // TODO - quaternions
    owner->GetCubeRenderer()->render(cube, owner->GetCamera());
}

bool Bomb::Exists() {
    return this->exists;
}