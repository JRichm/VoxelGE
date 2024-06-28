#include "Chunk.h"

Chunk::Chunk() {
	// initialize chunk here
}

Chunk::~Chunk() {
	// Destruction
}

void Chunk::setVoxel(const glm::vec3& position, const Voxel& voxel) {
	glm::ivec3 voxelPos = glm::floor(position);
	voxels[voxelPos] = voxel;
}


Voxel Chunk::getVoxel(const glm::vec3& position) const {
	glm::ivec3 voxelPos = glm::floor(position);
	auto it = voxels.find(voxelPos);
	if (it != voxels.end()) {
		return it->second;
	}
	else {
		// return default voxel or handle appropriately
		return Voxel(); // assuming Voxel() is a default constructor or default voxel
	}
}


// Chunk::Chunk() {
//     voxels.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
// }
// 
// Voxel& Chunk::getVoxel(int x, int y, int z) {
//     return voxels[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
// }
// 
// const Voxel& Chunk::getVoxel(int x, int y, int z) const {
//     return voxels[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
// }