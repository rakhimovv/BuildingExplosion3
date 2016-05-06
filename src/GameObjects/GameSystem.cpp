//
// Created by Руслан Рахимов on 06/05/16.
//

#include "GameSystem.h"
#include "../Physics/AosParticleSystem.h"

GameSystem::GameSystem(float constTimeStep) {
    this->constTimeStep = constTimeStep;
    this->particleSystem = new AosParticleSystem<ParticleInfo>(Vector3f(-1e5f, -1e5f, -1e5f),
                                                               Vector3f(1e5f, 1e5f, 1e5f),
                                                               constTimeStep);
    this->skyBoxRenderer = new SkyBoxRenderer();
    this->cubeRenderer = new CubeRenderer();

    // Создаем блоки
    for (int i = 0; i < 2; i++) {
        Block::Descriptor blockDesc;
        blockDesc.vertexPositions.push_back(Vector3f(-3.0f + 2 * i + 0.5f, 0.0f, 0.0f));
        blocks.Add(new Block(blockDesc, this));
    }
}

GameSystem::~GameSystem() {
    for (size_t blockIndex = 0; blockIndex < blocks.GetElementsCount(); blockIndex++) {
        delete blocks[blockIndex];
    }
    delete skyBoxRenderer;
    delete cubeRenderer;
}

ParticleSystem<ParticleInfo> *GameSystem::GetParticleSystem() {
    return particleSystem;
}

CubeRenderer *GameSystem::GetCubeRenderer() {
    return cubeRenderer;
}

SkyBoxRenderer *GameSystem::GetSkyBoxRenderer() {
    return skyBoxRenderer;
}

void GameSystem::Update(float dt) {
    // Удаляем несуществующие объекты
    for (size_t blockIndex = 0; blockIndex < blocks.GetElementsCount(); blockIndex++) {
        if (!blocks[blockIndex]->Exists()) {
            delete blocks[blockIndex];
            blocks[blockIndex] = 0;
            blocks.RemoveByIndex(blockIndex);
        }
    }

    // Обновляем CashedArray
    blocks.Update();

    // Обновляем систему частиц
    particleSystem->Update();

    // Вернуть дефолтное значение ускорения
    for (size_t particleIndex = 0; particleIndex < particleSystem->GetParticlesCount(); particleIndex++) {
        particleSystem->GetParticle(particleIndex).SetAcceleration(Vector3f(0.0f, 0.0f, 0.0f));
    }

    // Обновляем блоки
    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        blocks[objectIndex]->Update(dt);
    }

    // Отрисовка
    this->skyBoxRenderer->render();
    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        blocks[objectIndex]->Render();
    }
}