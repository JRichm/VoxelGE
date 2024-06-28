#pragma once

#include <string>

#include "octree.h"
#include "chunk.h"

class World {
public:
    World(glm::vec3 worldCenter, float worldSize, int maxOctreeDepth);
    ~World();

    void createChunk(int x, int y, int z);
    Chunk* getChunk(int x, int y, int z);
    Octree& getOctree();

    void setVoxel(const glm::vec3& position, const Voxel voxel);
    Voxel getVoxel(const glm::vec3& position) const;

private:
    Octree octree;
    std::unordered_map<std::string, Chunk> chunks;
};

