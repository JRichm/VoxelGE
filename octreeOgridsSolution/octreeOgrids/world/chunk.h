#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "voxel.h"

class Chunk
{
public:

	// maybe use sparse voxel structures if chunks tend to have many empty voxels

	int size; // size of the chunk 
	glm::vec3 position;
	std::vector<std::vector<std::vector<Voxel>>> voxels; // 3d array of voxels

	Chunk(int _size, const glm::vec3& _position) : size(_size), position(_position), voxels(_size, std::vector<std::vector<Voxel>>(_size, std::vector<Voxel>(_size))) {}

	bool insertVoxel(int x, int y, int z, const Voxel& voxel);
};

