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
    this->particleHandle = owner->GetParticleSystem()->AddParticle(desc.pos, 1.0f, false);
}

Bomb::~Bomb() {
    particleHandle.Release();
}

void Bomb::Update(float dt) {
    // TODO замутить движение бомбы или привязать к камере ее

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        this->exists = false;
        // ВЗРЫВ
        std::cout << "ВЗРЫВ!\n";

        Explosion::Descriptor explosionDesc;
        explosionDesc.maxRadius = 15.0f;
        explosionDesc.totalLifetime = 10.0f;
        explosionDesc.pos = particleHandle.GetPos();

        owner->SetExplosion(new Explosion(explosionDesc, owner));
    }
}

void Bomb::Render() {
    // TODO
    Vector3f pos = particleHandle.GetPos();
    Cube cube(glm::vec3(pos.x, pos.y, pos.z), 0.5f); // TODO - quaternions
    owner->GetCubeRenderer()->render(cube);
}

bool Bomb::Exists() {
    return this->exists;
}