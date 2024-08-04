#include <iostream>

#include <array>
#include <glm/glm.hpp>

#include "octree.h"
#include "chunk.h"

void Octree::subdivide(OctreeNode* node) {
	float halfSize = node->size / 2.0f;
	float quarterSize = node->size / 4.0f;

	std::array<glm::vec3, 8> childPositions = {
		node->position + glm::vec3(-quarterSize, -quarterSize, -quarterSize),
		node->position + glm::vec3(quarterSize, -quarterSize, -quarterSize),
		node->position + glm::vec3(-quarterSize, quarterSize, -quarterSize),
		node->position + glm::vec3(quarterSize, quarterSize, -quarterSize),
		node->position + glm::vec3(-quarterSize, -quarterSize, quarterSize),
		node->position + glm::vec3(quarterSize, -quarterSize, quarterSize),
		node->position + glm::vec3(-quarterSize, quarterSize, quarterSize),
		node->position + glm::vec3(quarterSize, quarterSize, quarterSize),
	};

	for (int i = 0; i < 8; ++i) {
		node->children[i] = std::make_unique<OctreeNode>(childPositions[i], halfSize);
	}
}

bool Octree::insert(OctreeNode* node, const Chunk& chunk, int depth) {
    // if the maximum depth is reached insert the chunk into the current node
    if (depth == maxDepth) {
        node->chunks.push_back(chunk);       // add chunk to the node's chunk list
        node->hasChunks = true;              // mark the node as containing chunks
        std::cout << "Chunk added at depth " << depth << " to node at position (" << node->position.x << ", " << node->position.y << ", " << node->position.z << ")" << std::endl;
        return true;
    }

    // if the current node has no children subdivide it into 8 child nodes
    if (node->children[0] == nullptr) {
        subdivide(node);
    }

    // Determine which child node the chunk belongs to based on the chunk's position
    for (auto& child : node->children) {
        glm::vec3 childMin = child->position - glm::vec3(child->size / 2.0f);
        glm::vec3 childMax = child->position + glm::vec3(child->size / 2.0f);

        if (
            chunk.position.x >= childMin.x && chunk.position.x <= childMax.x &&
            chunk.position.y >= childMin.y && chunk.position.y <= childMax.y &&
            chunk.position.z >= childMin.z && chunk.position.z <= childMax.z
            ) {
            return insert(child.get(), chunk, depth + 1);
        }
    }

    // return false if insertion fails
    return false;
}

bool Octree::insertVoxel(OctreeNode* node, const glm::vec3& position, const Voxel& voxel, int depth) {
	if (depth == maxDepth) {
        std::cout << "Max Depth" << std::endl;
        if (node->chunks.empty()) {
            std::cout << "Node has no chunks at max depth" << std::endl;
        }
        else {
            std::cout << "Node has " << node->chunks.size() << " chunks at max depth" << std::endl;
        }

        for (auto& chunk : node->chunks) {
            int x = static_cast<int>(((position.x - node->position.x + (node->size / 2.0f)) / node->size) * chunk.size);
            int y = static_cast<int>(((position.y - node->position.y + (node->size / 2.0f)) / node->size) * chunk.size);
            int z = static_cast<int>(((position.z - node->position.z + (node->size / 2.0f)) / node->size) * chunk.size);

            std::cout << "something" << std::endl;
            if (x >= 0 && x < chunk.size && y >= 0 && y < chunk.size && z >= 0 && z < chunk.size) {
                std::cout << "Local coordinates in chunk bounds: (" << x << ", " << y << ", " << z << ")" << std::endl;
                if (chunk.insertVoxel(x, y, z, voxel)) {
                    std::cout << "Voxel inserted at local coordinates: (" << x << ", " << y << ", " << z << ")" << std::endl;
                    return true;
                }
                else {
                    std::cout << "Failed to insert voxel at local coordinates: (" << x << ", " << y << ", " << z << ")" << std::endl;
                }
            }
            else {
                std::cout << "Local coordinates out of bounds: (" << x << ", " << y << ", " << z << ")" << std::endl;
            }
        }
    }
    else {
        if (node->children[0] == nullptr) {
            subdivide(node);
        }

        for (auto& child : node->children) {
            glm::vec3 childMin = child->position - glm::vec3(child->size / 2.0f);
            glm::vec3 childMax = child->position + glm::vec3(child->size / 2.0f);

            if (
                position.x >= childMin.x && position.x <= childMax.x &&
                position.y >= childMin.y && position.y <= childMax.y &&
                position.z >= childMin.z && position.z <= childMax.z
                ) {
                std::cout << "Inserting voxel into child node at depth " << depth + 1 << std::endl;
                return insertVoxel(child.get(), position, voxel, depth + 1);
            }
        }
    }
    return false;
}

void Octree::gatherVoxels(std::vector<glm::vec3>& positions, std::vector<Voxel>& voxels, OctreeNode* node) {
    // if (!node) return;
    // 
    // if (node->hasChunks) {
    //     for (const auto& chunk : node->chunks) {
    //         for (int x = 0; x < chunk.size; ++x) {
    //             for (int y = 0; y < chunk.size; ++y) {
    //                 for (int z = 0; z < chunk.size; ++z) {
    //                     if (chunk.voxels[x][y][z].type != 0) {
    //                         positions.push_back(chunk.position + glm::vec3(x, y, z));
    //                         voxels.push_back(chunk.voxels[x][y][z]);
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // for (auto& child : node->children) {
    //     gatherVoxels(positions, voxels, child.get());
    // }
}

void Octree::printNode(OctreeNode* node, int depth) {
	std::cout << "Node at depth " << depth << " with position (" << node->position.x << ", " << node->position.y << ", " << node->position.z << ") and size " << node->size << std::endl;
	if (node->hasChunks) {
		std::cout << "  Contains chunks" << std::endl;
	}
	for (auto& child : node->children) {
		if (child) {
			printNode(child.get(), depth + 1);
		}
	}
}