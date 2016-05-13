//
// Created by aviator on 5/11/16.
//

#ifndef BUILDINGEXPLOSION3_MESH_H
#define BUILDINGEXPLOSION3_MESH_H

#include <vector>

#include "Camera.h"

class Mesh {
virtual std::vector<float>& getVertices() = 0;
virtual std::vector<int>& getIndexes() = 0;
virtual void render(Camera& camera) = 0;
};


#endif //BUILDINGEXPLOSION3_MESH_H
