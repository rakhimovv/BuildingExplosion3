#pragma once
#include "Line.h"
#include "../Utils/CachedArray.h"
#include "Block.h"

class Block;

class Building {
public:
    struct LinkLine {
        size_t p0;
        size_t p1;
        Line * line;
    };

    enum Type {
        Cylinder,
        Hyperboloid
    };

    Building(Type buildingType, Vector3f minPoint, GameSystem * owner, float edge, int N, int H, float R1);

    ~Building();

    void Update(float dt);

    void Render(GameSystem *owner, Camera * camera);

    bool Exists();

private:

    CachedArray<LinkLine> linkLine;
    CachedArray<Block *> blocks;
};
