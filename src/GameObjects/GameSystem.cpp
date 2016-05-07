//
// Created by Руслан Рахимов on 06/05/16.
//

#include "GameSystem.h"
#include "../Physics/AosParticleSystem.h"

class LineRenderer;
// Границы мира
const Vector3f minPoint = Vector3f(-15.0f, -1.0f, -15.0f);
const Vector3f maxPoint = Vector3f(15.0f, 15.0f, 15.0f);

GameSystem::GameSystem(float constTimeStep) {

    this->constTimeStep = constTimeStep;
    this->particleSystem = new AosParticleSystem<ParticleInfo>(minPoint, maxPoint, constTimeStep);
    this->explosion = 0;
    this->bomb = 0;

    // Инициалиазация openGl для примитивов в классах обернута
    this->skyBoxRenderer = new SkyBoxRenderer;
    this->cubeRenderer = new CubeRenderer;
    this->lineRenderer = new LineRenderer;


    // Создаем блоки
    /*
    for (int i = 0; i < 2; i++) {
        Block::Descriptor blockDesc;
        // TODO как задавать блоки
        float edge = 0.25f;
        blockDesc.vertexPositions.push_back(Vector3f(-1.0f + edge * i, 1.0f, 0.0f));
        blocks.Add(new Block(blockDesc, this));
    }

    // Добавим связи между блоками (не внутри них!)
    for (int i = 0; i < blocks.GetElementsCount(); i++) {
        // TODO нормально добавиться связи
        for (int j = 0; j < blocks.GetElementsCount(); j++) {
            if (i != j) {
                GetParticleSystem()->AddLink(*blocks.GetByIndex(i)->GetParticleHandle(0), *blocks.GetByIndex(j)->GetParticleHandle(0));
            }
        }
    }
    */

    //h этажей по n кубиков
    int h = 5;
    int n = 10;
    float edge = 0.2f;
    float empty = edge / 4.0f; // промежуток между кубиками

    // левая стенка
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < n; j++) {
            Block::Descriptor blockDesc;
            Vector3f pos = Vector3f(-1.0f, minPoint.y + edge / 2.0f + i * (edge + empty), -1.0f + edge / 2.0f + j * (edge + empty));
            //pos.Print(); std::cout << std::endl;
            blockDesc.vertexPositions.push_back(pos);
            blockDesc.edgeLength = edge;
            blocks.Add(new Block(blockDesc, this));
        }
    }

    // правая стенка
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < n; j++) {
            Block::Descriptor blockDesc;
            Vector3f pos = Vector3f(-1.0f + 3 * edge, minPoint.y + edge / 2.0f + i * (edge + empty), -1.0f + edge / 2.0f + j * (edge + empty));
            pos.Print(); std::cout << std::endl;
            blockDesc.vertexPositions.push_back(pos);
            blockDesc.edgeLength = edge;
            blocks.Add(new Block(blockDesc, this));
        }
    }


    // Создадим бомбу
    Bomb::Descriptor bombDesc;
    bombDesc.edgeLength = 2.0f * edge;
    bombDesc.pos = Vector3f(-1.0f + 1.5f * edge, minPoint.y + edge / 2.0f + h / 2.0f * (edge + empty), -1.0f + edge / 2.0f + n / 2.0f * (edge + empty));
    bomb = new Bomb(bombDesc, this);
}

GameSystem::~GameSystem() {
    for (size_t blockIndex = 0; blockIndex < blocks.GetElementsCount(); blockIndex++) {
        delete blocks[blockIndex];
    }
    delete skyBoxRenderer;
    delete cubeRenderer;
//    delete lineRenderer;
    delete bomb;
    delete explosion;
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

void GameSystem::SetExplosion(Explosion *explosion) {
    this->explosion = explosion;
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

    // Обновить бомбу
    if (bomb && bomb->Exists()) {
        //std::cout << "Update bomb\n";
        bomb->Update(dt);
    } else {
        delete bomb;
        bomb = 0;
    }

    // Взрыв
    if (explosion && explosion->Exists()) {
        explosion->Update(dt);
    } else {
        delete explosion;
        explosion = 0;
    }

//     Отрисовка
    glm::vec3 p0(0.0f, 0.0f, 0.0f);
    glm::vec3 p1(0.0f, 1.0f, 0.0f);
    this->skyBoxRenderer->render();
    this->lineRenderer->render(p0, p1);
//    this->lineRenderer->render(new glm::vec3(0.0f, 0.0f, 0.0f), new glm::vec3(0.0f, 1.0f, 0.0f));
    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        blocks[objectIndex]->Render();
    }
    if (bomb && bomb->Exists()) {
        bomb->Render();
    }
    if (explosion && explosion->Exists()) {
        explosion->Render();
    }
}