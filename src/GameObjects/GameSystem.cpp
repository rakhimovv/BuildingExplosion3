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

GameSystem::GameSystem(float constTimeStep) : gameParameters("data/gameconfig.json") {
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

    // this->sphere = new Sphere(&center, 0.5f, &color, gameParameters);

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

            //template<typename UserInfo>
            auto jHandle = *blocks.GetByIndex(j)->GetParticleHandle(0);
            Vector3f jPos = jHandle.GetPos();

            //template<typename UserInfo>
            auto iHandle = *blocks.GetByIndex(i)->GetParticleHandle(0);
            Vector3f iPos = iHandle.GetPos();

            if ((jPos - iPos).Length() < std::sqrt(2.0f) * empty) {
                //LinkLine * l= new LinkLine();
                LinkLine l;
                GetParticleSystem()->AddLink(iHandle, jHandle, 0.001f, 1.0f);

                l.p0 = iHandle.GetParticleIndex();//this->particleSystem->GetLinks().back().particleId0;
                l.p1 = jHandle.GetParticleIndex();


                glm::vec3 pos0(iPos.x, iPos.y, iPos.z);
                glm::vec3 pos1(jPos.x, jPos.y, jPos.z);
                Line line(pos0, pos1, color, gameParameters);

                // TODO Здесь проблема с передачей твоего line в мою структуру. Структура LinkLine хранит номера
                // частиц к которым привязан link и класс line ответственный за отрисовку линии между ними
                l.line = line;

                linkLine.Add(l);
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

    /*
    for (size_t blockIndex = 0; blockIndex < linkLine.GetElementsCount(); blockIndex++) {
        delete linkLine[blockIndex];
    }*/

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

Camera *GameSystem::GetCamera() {
    return camera;
}

void GameSystem::SetExplosion(Explosion *explosion) {
    this->explosion = explosion;
}

void GameSystem::Update(float dt, std::queue<sf::Keyboard::Key> &pressedButtons) {
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
    linkLine.Update();

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

    for (size_t i = 0; i < linkLine.GetElementsCount(); i++) {
        Vector3f pos0 = this->particleSystem->GetParticleById(linkLine[i].p0).pos;
        Vector3f pos1 = this->particleSystem->GetParticleById(linkLine[i].p1).pos;
        glm::vec3 gpos0(pos0.x, pos0.y, pos0.z);
        glm::vec3 gpos1(pos1.x, pos1.y, pos1.z);

        linkLine[i].line.update(&gpos0, &gpos1);
        linkLine[i].line.render(*this->camera);
    }

    if (bomb && bomb->Exists()) {
        bomb->Render();
    }
    if (explosion && explosion->Exists()) {
        explosion->Render();
    }
}
