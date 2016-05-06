//
// Created by Руслан Рахимов on 06/05/16.
//

#include "Block.h"

Block::Descriptor::Descriptor() { }

Block::Block(const Descriptor &desc, GameSystem *owner) {
    this->exists = true;
    this->owner = owner;

    // Создаем частицы
    for (int i = 0; i < desc.vertexPositions.size(); i++) {
        particleHandles.push_back(owner->GetParticleSystem()->AddParticle(desc.vertexPositions[i], 5.0f, false));
    }

    // Добавляем гравитацию
    for (int i = 0; i < particleHandles.size(); i++) {
        this->particleHandles[i].SetAcceleration(Vector3f(0.0f, -1.0f, 0.0f));
    }

    // Добавляем связи
    for (int i = 0; i < particleHandles.size(); i++) {
        for (int j = 0; j < particleHandles.size(); j++) {
            if (i != j) {
                this->owner->GetParticleSystem()->AddLink(this->particleHandles[i], this->particleHandles[j]);
            }
        }
    }
}

Block::~Block() {
    for (int i = 0; i < particleHandles.size(); i++) {
        particleHandles[i].Release();
    }
    // TODO освободить линки
    // можно при апдейте линков смотреть существует ли частица и если нет удалять ее
}

void Block::Update(float dt) {

}

void Block::Render() {
    // TODO render
}

bool Block::Exists() {
    return exists;
}