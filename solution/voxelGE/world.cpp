#include <iostream>

#include "world.h"


World::World(glm::vec3 worldCenter, float worldSize, int maxOctreeDepth)
	: octree(worldCenter, worldSize, maxOctreeDepth) {
	// Constructor initializes octree with specified parameters
}

World::~World() {
	// Destructor
}


void World::createChunk(int x, int y, int z) {
	std::string key = std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
	Chunk newChunk;
	chunks[key] = newChunk;
	octree.insertVoxel(glm::vec3(x, y, z), Voxel());
}

Chunk* World::getChunk(int x, int y, int z) {
	std::string key = std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
	if (chunks.find(key) != chunks.end()) {
		return &chunks[key];
	}
	else {
		return nullptr;
	}
}

Octree& World::getOctree() {
	return octree;
}

void World::setVoxel(const glm::vec3& position, const Voxel& voxel) {
	Chunk* chunk = getChunk(position.x, position.y, position.z);
	if (chunk) {
		chunk->setVoxel(position, voxel);
	}
}

Voxel World::getVoxel(const glm::vec3& position) const {
	Chunk* chunk = getChunk(position.x, position.y, position.z);
	if (chunk) {
		return chunk->getVoxel(position);
	}
	else {
		// Return default voxel (or handle appropriately)
		return Voxel();
	}
}










//  
//  std::unordered_map<std::string, Chunk>& World::getAllChunks() {
//  	return chunks;
//  }
//  
//  void World::setVoxel(const glm::vec3& position, const Voxel voxel) {
//  	//octree.insertVoxel(voxel);
//  	std::cout << "octree.insertVoxel(voxel)" << std::endl;
//  }
//  
//  
//  Voxel World::getVoxel(const glm::vec3& position) const {
//  	glm::vec3 relativePosition = position - octree.getRoot()->getCenter();
//  
//  	// start traversal from the octree root
//  	OctreeNode* currentNode = octree.getRoot();
//  	int currentDepth = 0;
//  
//  	while (currentNode != nullptr && currentDepth < octree.getMaxDepth()) {
//  		// determine octant index based on relative position
//  		int octantIndex = currentNode->getOctantIndex(relativePosition);
//  
//  		// check if node exists
//  		if (currentNode->getChild(octantIndex) == nullptr) {
//  			// No child node, return default voxel (could be air or another default)
//  			return Voxel(); // placeholder return
//  		}
//  
//  		currentNode = currentNode->getChild(octantIndex);
//  		relativePosition = currentNode->transformWorldToLocal(relativePosition);
//  		++currentDepth;
//  	}
//  
//  	// at leaf node, retrieve voxels and find the voxel at the exact position 
//  	const std::vector<Voxel>& voxels = currentNode->getVoxels();
//  	for (const auto& voxel : voxels) {
//  		// check if voxel position matches the queried position
//  		// you need to define a mthod in Voxel class to get the position
//  		if (voxel.getPosition() == position) {
//  			return voxel;
//  		}
//  	}
//  
//  	// if voxel not fou nd (should not occur if octree construction is correct
//  	return Voxel(); // placeholder return
//  }