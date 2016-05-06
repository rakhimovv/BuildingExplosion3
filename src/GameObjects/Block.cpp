//
// Created by Руслан Рахимов on 06/05/16.
//

#include "Block.h"
#include "../Cube.h"

Block::Descriptor::Descriptor() { }

Block::Block(const Descriptor &desc, GameSystem *owner) {
    this->exists = true;
    this->owner = owner;

    // Создаем частицы
    for (int i = 0; i < desc.vertexPositions.size(); i++) {
        particleHandles.push_back(owner->GetParticleSystem()->AddParticle(desc.vertexPositions[i], 1.0f, false));
    }

    /*
    // Добавляем связи
    for (int i = 0; i < particleHandles.size(); i++) {
        for (int j = 0; j < particleHandles.size(); j++) {
            if (i != j) {
                this->owner->GetParticleSystem()->AddLink(this->particleHandles[i], this->particleHandles[j]);
            }
        }
    }
     */
}

Block::~Block() {
    for (int i = 0; i < particleHandles.size(); i++) {
        particleHandles[i].Release();
    }
    // TODO освободить линки
    // можно при апдейте линков смотреть существует ли частица и если нет удалять ее
}

void Block::Update(float dt) {
    // Добавляем гравитацию
    for (int i = 0; i < particleHandles.size(); i++) {
        this->particleHandles[i].SetAcceleration(Vector3f(0.0f, -0.1f, 0.0f));
    }
}

void Block::Render() {
    // TODO додумать как отрисовывать блок
    Vector3f pos = particleHandles[0].GetPos();
    Cube cube(glm::vec3(pos.x, pos.y, pos.z), 1.0f); // TODO - quaternions
    owner->GetCubeRenderer()->render(cube);
}

bool Block::Exists() {
    return exists;
}

ParticleHandle<ParticleInfo> *Block::GetParticleHandle(int id) {
    return  &(particleHandles[id]);
}