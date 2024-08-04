#include <iostream>

#include <glm/glm.hpp>

#include "octree.h"
#include "chunk.h"
#include "world.h"


World::World(const int _maxDepth, const float _rootSize) : maxDepth(_maxDepth), rootSize(_rootSize) {

	glm::vec3 rootPosition(0.0f, 0.0f, 0.0f);

	octree.maxDepth = maxDepth;
	octree.root = std::make_unique<OctreeNode>(rootPosition, rootSize);
}

bool World::InsertChunk(Chunk chunk) {
	if (octree.insert(octree.root.get(), chunk, 0)) {
        std::cout << "Chunk inserted successfully" << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to insert chunk" << std::endl;
        return false;
    }
}

bool World::InsertVoxel(Voxel voxel, glm::vec3 voxelPosition) {
    if (octree.insertVoxel(octree.root.get(), voxelPosition, voxel, 0)) {
        std::cout << "Voxel inserted successfully" << std::endl;
        return true;
    }
    else {
        std::cout << "Failed to insert voxel" << std::endl;
        return false;
    }
}