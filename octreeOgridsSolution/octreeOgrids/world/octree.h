#pragma once
#include <memory>


#include <vector>

#include "voxel.h"
#include "chunk.h"
#include <glm/glm.hpp>

class OctreeNode {
public:
	std::vector<Chunk> chunks;
	bool hasChunks;
	std::vector<std::unique_ptr<OctreeNode>> children;
	glm::vec3 position;
	float size;

	//OctreeNode() : hasChunks(false), children(8) {}
	OctreeNode(const glm::vec3& pos, float _size) : hasChunks(false), children(8), position(pos), size(_size) {}
	
};

class Octree {
public:
	int maxDepth;
	std::unique_ptr<OctreeNode> root;

	void subdivide(OctreeNode* node);
	bool insert(OctreeNode* node, const Chunk& chunk, int depth);
	bool insertVoxel(OctreeNode* node, const glm::vec3& position, const Voxel& voxel, int depth);
	void printNode(OctreeNode* node, int depth);
	void gatherVoxels(std::vector<glm::vec3>& positions, std::vector<Voxel>& voxels, OctreeNode* node);
};