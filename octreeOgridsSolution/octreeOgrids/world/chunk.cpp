#include "chunk.h"

#include "voxel.h"

bool Chunk::insertVoxel(int x, int y, int z, const Voxel& voxel) {
    if (x >= 0 && x < size && y >= 0 && y < size && z >= 0 && z < size) {
		voxels[x][y][z] = voxel;
		return true;
	}
	return false;
}