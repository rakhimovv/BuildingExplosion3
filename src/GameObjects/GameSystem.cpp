//
// Created by Руслан Рахимов on 06/05/16.
//

#include "GameSystem.h"
#include "../Physics/AosParticleSystem.h"
#include "Line.h"

class LineRenderer;

// Границы мира
const Vector3f minPoint = Vector3f(-15.0f, -1.0f, -15.0f);
const Vector3f maxPoint = Vector3f(15.0f, 15.0f, 15.0f);

GameSystem::GameSystem(float constTimeStep): gameParameters("data/gameconfig.json") {
    // load config

    this->constTimeStep = constTimeStep;
    this->particleSystem = new AosParticleSystem<ParticleInfo>(minPoint, maxPoint, constTimeStep);
    this->explosion = nullptr;
    this->bomb = nullptr;
    this->camera = new Camera(gameParameters);

    // Инициалиазация openGl для примитивов в классах обернута
    this->skyBoxRenderer = new SkyBoxRenderer(gameParameters);
    this->cubeRenderer = new CubeRenderer(gameParameters);

    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

//    this->sphere = new Sphere(&center, 0.5f, &color, gameParameters);

    /*
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

    // Улетают в бесконечность, потому что нет гравитации и границ мира, кроме нижней

    //h этажей по n кубиков
    int h = 2;
    int n = 2;
    float edge = 0.1f;
    float empty = edge * 4.0f; // промежуток между кубиками

    // левая стенка
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < n; j++) {
            Block::Descriptor blockDesc;
            Vector3f pos = Vector3f(-1.0f, minPoint.y + edge / 2.0f + i * empty,
                                    -1.0f + edge / 2.0f + j * empty);
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
            Vector3f pos = Vector3f(-1.0f + 3 * edge, minPoint.y + edge / 2.0f + i * empty,
                                    -1.0f + edge / 2.0f + j * empty);
            //pos.Print(); std::cout << std::endl;
            blockDesc.vertexPositions.push_back(pos);
            blockDesc.edgeLength = edge;
            blocks.Add(new Block(blockDesc, this));
        }
    }


    // Добавим связи между блоками (не внутри них!)


    for (int i = 0; i < blocks.GetElementsCount(); i++) {
        for (int j = 0; j < blocks.GetElementsCount(); j++) {
            //ParticleHandle * jHandle = blocks.GetByIndex(j)->GetParticleHandle(0);
            //Vector3f jPos = blocks.GetByIndex(j)->GetParticleHandle(0)->GetPos();

            //ParticleHandle * iHandle = blocks.GetByIndex(i)->GetParticleHandle(0);
            //Vector3f iPos = blocks.GetByIndex(i)->GetParticleHandle(0)->GetPos();

            //if ((jPos - iPos).Length() < std::sqrt(2.0f) * empty) {
            if (i != j) {
                GetParticleSystem()->AddLink(*blocks.GetByIndex(i)->GetParticleHandle(0), *blocks.GetByIndex(j)->GetParticleHandle(0), 0.001f, 1.0f);
            }
        }
    }

    // Создадим бомбу
    Bomb::Descriptor bombDesc;
    bombDesc.edgeLength = 2.0f * edge;
    bombDesc.pos = Vector3f(-1.0f + 1.5f * edge, minPoint.y + edge / 2.0f + h / 2.0f * empty,
                            -1.0f + edge / 2.0f + n / 2.0f * empty);
    bomb = new Bomb(bombDesc, this);

}

GameSystem::~GameSystem() {
    for (size_t blockIndex = 0; blockIndex < blocks.GetElementsCount(); blockIndex++) {
        delete blocks[blockIndex];
    }

    delete skyBoxRenderer;
    delete cubeRenderer;
    delete bomb;
    delete camera;
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

Camera *GameSystem::GetCamera()
{
    return camera;
}
void GameSystem::SetExplosion(Explosion *explosion) {
    this->explosion = explosion;
}

void GameSystem::Update(float dt, std::queue<sf::Keyboard::Key>& pressedButtons) {
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
        //std::cout << "нет взрыва\n";
        delete explosion;
        explosion = 0;
    }

    // Отрисовка
    camera->MoveCamera(pressedButtons);

    this->skyBoxRenderer->render(this->camera);

    //this->sphere->render(*this->camera);

    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        blocks[objectIndex]->Render();
    }

    /* TODO КРАШ
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
    for (size_t i = 0; i < this->particleSystem->GetLinks().size(); i++) {
        size_t particleId0 = this->particleSystem->GetLinks()[i].particleId0;
        size_t particleId1 = this->particleSystem->GetLinks()[i].particleId1;
        Vector3f pos0 = this->particleSystem->GetParticle(particleId0).GetPos();
        Vector3f pos1 = this->particleSystem->GetParticle(particleId1).GetPos();
        glm::vec3 p0(pos0.x, pos0.y, pos0.z);
        glm::vec3 p1(pos1.x, pos1.y, pos1.z);
        Line line(p0, p1, color, gameParameters);
        this->lineRenderer->render(line, *this->camera);
    }
    */

    if (bomb && bomb->Exists()) {
        bomb->Render();
    }
    if (explosion && explosion->Exists()) {
        explosion->Render();
    }
}
