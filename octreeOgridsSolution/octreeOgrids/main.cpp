#include <iostream>

#include <glm/glm.hpp>

#include "world/octree.h"
#include "world/chunk.h"
#include "world/world.h"
#include "world/voxel.h"

#include "rendering/renderer.h"

int main() {
	std::cout << "Starting VoxGE..." << std::endl;
	World world = World(4, 64);

	glm::vec3 chunkPosition(-18.0f, -18.0f, -18.0f); // Ensure chunk has a position
	Chunk chunk(64, chunkPosition);
	world.InsertChunk(chunk);

	Voxel voxel(1);
	glm::vec3 voxelPosition(-18.0f, -18.0f, -18.0f);

	world.InsertVoxel(voxel, voxelPosition);

	//world.octree.printNode(world.octree.root.get(), 0);

	Renderer renderer = Renderer(1280, 720);
	renderer.initglfw();

	std::vector<glm::vec3> positions;
	std::vector<Voxel> voxels;

	world.octree.gatherVoxels(positions, voxels, world.octree.root.get());

	renderer.createVoxelBuffers(positions, voxels);

	while (renderer.running) {
		renderer.update();
	}

	renderer.terminate();

	return 0;
}