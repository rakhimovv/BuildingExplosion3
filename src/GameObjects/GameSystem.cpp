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

    this->gameGraphic = new GameGraphic(gameParameters);
    // Инициалиазация openGl для примитивов в классах обернута
    this->skyBoxRenderer = new SkyBoxRenderer(gameGraphic->GetSkyboxShader(), gameParameters);
    this->cubeRenderer = new CubeRenderer(gameGraphic->GetCubeShader(), gameParameters);

    float edge = 0.05f;

    int H = 3;
    int N = 35;
    float R1 = 0.8f;
    //building = new Building(Building::Type::Cylinder, minPoint, this, edge, N, H, R1);

//    int H = 2;
//    int N = 100;
//    float R1 = 1.3f;
//    building = new Building(Building::Type::Hyperboloid, minPoint, this, edge, N, H, R1);

    // Создадим бомбу
    /*
    Bomb::Descriptor bombDesc;
    bombDesc.edgeLength = 2.0f * edge;
    //bombDesc.pos = Vector3f(0.0f, H / 3.0f, 0.0f);
    bombDesc.pos = Vector3f(0.0f, 1.5f, 0.0f);
    bomb = new Bomb(bombDesc, this);
     */
}

GameSystem::~GameSystem() {
    camera->PrintParameters();

    delete building;
    delete bomb;
    delete gameGraphic;
    delete skyBoxRenderer;
    delete cubeRenderer;
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

void GameSystem::OnCylinder() {
    delete building;
    delete bomb;
    delete explosion;
    building = 0;
    bomb = 0;
    explosion = 0;
    float edge = 0.05f;
    int H = 3;
    int N = 35;
    float R1 = 0.8f;
    building = new Building(Building::Type::Cylinder, minPoint, this, edge, N, H, R1);
}

void GameSystem::OnHyperboloid() {
    delete building;
    delete bomb;
    delete explosion;
    building = 0;
    bomb = 0;
    explosion = 0;
    float edge = 0.05f;
    int H = 2;
    int N = 100;
    float R1 = 1.3f;
    building = new Building(Building::Type::Hyperboloid, minPoint, this, edge, N, H, R1);
}

void GameSystem::OnNewBomb(float force) {
    delete explosion;
    delete bomb;
    explosion = 0;
    bomb = 0;
    float edge = 0.05f;
    Bomb::Descriptor bombDesc;
    bombDesc.edgeLength = 2.0f * edge;
    //bombDesc.pos = Vector3f(0.0f, H / 3.0f, 0.0f);
    bombDesc.pos = Vector3f(0.0f, 1.5f, 0.0f);
    bombDesc.force = force;
    bomb = new Bomb(bombDesc, this);
}

void GameSystem::OnBoom() {
    if (bomb && bomb->Exists()) {
        bomb->boom = true;
    } else {
        delete bomb;
        bomb = 0;
    }
}

void GameSystem::OnExit() {
    GameSystem::~GameSystem();
    exit(0);
}

void GameSystem::SetExplosion(Explosion *explosion) {
    this->explosion = explosion;
}

void GameSystem::Update(float dt, std::queue<sf::Keyboard::Key> &pressedButtons) {

    // Обновляем систему частиц
    particleSystem->Update();

    //std::cout << "\n\n\n\n\n";

    // Вернуть дефолтное значение ускорения
    for (size_t particleIndex = 0; particleIndex < particleSystem->GetParticlesCount(); particleIndex++) {
        particleSystem->GetParticle(particleIndex).SetAcceleration(Vector3f(0.0f, 0.0f, 0.0f));
    }

    // Обновляем здание
    if (building) {
        building->Update(dt, this);
    }

    if (bomb && bomb->Exists()) {
        bomb->Boom(dt);
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

    // Отрисовка
    camera->MoveCamera(pressedButtons);

    this->skyBoxRenderer->render(this->camera);

    if (building) {
        building->Render(this, camera);
    }

    if (bomb && bomb->Exists()) {
        bomb->Render();
    }
    if (explosion && explosion->Exists()) {
        explosion->Render();
    }
}

GameGraphic * GameSystem::GetGameGraphic() {
    return gameGraphic;
}
