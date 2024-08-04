#pragma once

#include "octree.h"

class World
{
public:

    int maxDepth;
    float rootSize;
    Octree octree;
    World(const int _maxDepth, const float _rootSize);
    bool InsertChunk(Chunk chunk);
    bool InsertVoxel(Voxel voxel, glm::vec3 voxelPosition);
};

