//
// Created by Руслан Рахимов on 20/05/16.
//

#include "Building.h"

Building::Building(Building::Type buildingType, Vector3f minPoint, GameSystem *owner, float edge, int N, int H, float R1) {
    switch (buildingType) {
        case Type::Cylinder: {
            //int H = 7;
            //int N = 7;
            //float R1 = 0.4f;
            float R2 = 0.7f;
            //float edge = 0.05f;
            double alpha = 2.0f * M_PI / (N - 1.0f);

            double a = R1 * sin(alpha) / sin(90.0 - alpha / 2.0);

            for (float z = 0; z < H; z += a) {
                for (int i = 0; i < N; i++) {
                    double beta = alpha * (i * 1.0f);
                    float x = cosf(beta);
                    float y = sinf(beta);

                    Block::Descriptor blockDesc;
                    Vector3f pos = Vector3f(x * R1, minPoint.y + edge / 2.0f + z, y * R1);
                    blockDesc.vertexPositions.push_back(pos);
                    blockDesc.edgeLength = edge;
                    blocks.Add(new Block(blockDesc, owner));
                }
            }

            for (int i = 0; i < blocks.GetElementsCount(); i++) {

                auto iHandle = *blocks.GetByIndex(i)->GetParticleHandle(0);
                Vector3f iPos = iHandle.GetPos();

                for (int j = 0; j < blocks.GetElementsCount(); j++) {
                    auto jHandle = *blocks.GetByIndex(j)->GetParticleHandle(0);
                    Vector3f jPos = jHandle.GetPos();

                    //if (i != j && (jPos - iPos).Length() < std::sqrt(2.0f) * empty + 0.1) {
                    //if (i != j) {
                    if (i != j && (jPos - iPos).Length() <= a + 0.2f) {
                        LinkLine l;

                        l.linkId = owner->GetParticleSystem()->AddLink(iHandle, jHandle, 0.2f, 1.0f);
                        l.p0 = iHandle.GetParticleIndex();//this->particleSystem->GetLinks().back().particleId0;
                        l.p1 = jHandle.GetParticleIndex();
                        l.exists = true;

                        glm::vec3 pos0(iPos.x, iPos.y, iPos.z);
                        glm::vec3 pos1(jPos.x, jPos.y, jPos.z);

                        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

                        l.line = new Line(pos0, pos1, color, owner->GetGameGraphic()->GetLineShader());

                        linkLine.Add(l);
                    }
                }
            }

            break;
        }
        case Type::Hyperboloid: {
            //h этажей по n кубиков
            /*
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
                    Vector3f pos = Vector3f(-1.0f + empty, minPoint.y + edge / 2.0f + i * empty,
                                            -1.0f + edge / 2.0f + j * empty);
                    //pos.Print(); std::cout << std::endl;
                    blockDesc.vertexPositions.push_back(pos);
                    blockDesc.edgeLength = edge;
                    blocks.Add(new Block(blockDesc, this));
                }
            }*///h этажей по n кубиков
            /*
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
                    Vector3f pos = Vector3f(-1.0f + empty, minPoint.y + edge / 2.0f + i * empty,
                                            -1.0f + edge / 2.0f + j * empty);
                    //pos.Print(); std::cout << std::endl;
                    blockDesc.vertexPositions.push_back(pos);
                    blockDesc.edgeLength = edge;
                    blocks.Add(new Block(blockDesc, this));
                }
            }*/
            break;
        }
    }
}

Building::~Building() {
    for (size_t blockIndex = 0; blockIndex < blocks.GetElementsCount(); blockIndex++) {
        delete blocks[blockIndex];
    }

    for (size_t blockIndex = 0; blockIndex < linkLine.GetElementsCount(); blockIndex++) {
        delete linkLine[blockIndex].line;
    }
}

void Building::Update(float dt, GameSystem * owner) {
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

    // Обновляем блоки
    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        blocks[objectIndex]->Update(dt);
    }

    // Если связь оборвалась => меняем цвет
    for (size_t i = 0; i < linkLine.GetElementsCount(); i++) {
        if (linkLine[i].exists && !((AosParticleSystem<ParticleInfo> *) owner->GetParticleSystem())->IsLinkExists(linkLine[i].linkId)) {
            //std::cout << "Update color: " << linkLine[i].linkId << "\n";
            //linkLine[i].line->dump();
            linkLine[i].exists = 0;
            glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);
            linkLine[i].line->updateColor(&color);
        }
    }
}

void Building::Render(GameSystem *owner, Camera * camera) {
    //template<typename UserInfo>
    //AosParticleSystem<ParticleInfo> * particleSystem = (AosParticleSystem) owner->GetParticleSystem();

    for (size_t objectIndex = 0; objectIndex < blocks.GetElementsCount(); objectIndex++) {
        //blocks[objectIndex]->Render();
    }

    for (size_t i = 0; i < linkLine.GetElementsCount(); i++) {
        Vector3f pos0 = ((AosParticleSystem<ParticleInfo> *) owner->GetParticleSystem())->GetParticleById(linkLine[i].p0).pos;
        Vector3f pos1 = ((AosParticleSystem<ParticleInfo> *) owner->GetParticleSystem())->GetParticleById(linkLine[i].p1).pos;
        glm::vec3 gpos0(pos0.x, pos0.y, pos0.z);
        glm::vec3 gpos1(pos1.x, pos1.y, pos1.z);

        linkLine[i].line->update(&gpos0, &gpos1);
        linkLine[i].line->render(*camera);
    }
}